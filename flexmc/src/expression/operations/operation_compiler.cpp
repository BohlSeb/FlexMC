#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <fmt/format.h>

#include "functions_real.h"
#include "operators_calc.h"
#include "operation_compiler.h"

namespace flexMC
{
    Operation functionCompiler::compile(const size_t &num_args, Operands &stacks, MaybeError &report)
    {
        assert(stacks.fSize() > 0);
        const Token fun = stacks.funcsBack();

        using namespace functionsReal;
        auto is_scalar = std::find(symbols_scalar.cbegin(), symbols_scalar.cend(), fun.value) != symbols_scalar.cend();
        auto is_reduce = std::find(symbols_reduce.cbegin(), symbols_reduce.cend(), fun.value) != symbols_reduce.cend();
        assert(is_scalar || is_reduce);

        stacks.popFunc();
        if (is_scalar)
        {
            return functionCompiler::detail::compileScalar(fun, num_args, stacks, report);
        }
        if (is_reduce)
        {
            return functionCompiler::detail::compileReduce(fun, num_args, stacks, report);
        }
        return Operation(std::function<void(CalcStacks &)>{});
    }

    Operation
    functionCompiler::detail::compileScalar(const Token &function,
                                            const size_t &num_args,
                                            Operands &stacks,
                                            MaybeError &report)
    {
        assert(stacks.tSize() >= num_args);
        functions::assertNumberOfArgs(function, 1, num_args, report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        const Operands::Type return_type = functionsReal::compileArgType(function, stacks.typesBack(), report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        const std::function<void(CalcStacks &)> call_back = functionsReal::scalarFunc(function.value, return_type);
        return Operation(call_back);
    }

    Operation
    functionCompiler::detail::compileReduce(const Token &function,
                                            const size_t &num_args,
                                            Operands &stacks,
                                            MaybeError &report)
    {
        assert(stacks.tSize() >= num_args);
        const Operands::Type arg_type = functionsReal::compileArgType(function, stacks.typesBack(), report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        stacks.popType();
        if (arg_type == Operands::Type::scalar)
        {
            functions::assertNumberOfArgs(function, 2, 0, num_args, report);
            if (report.isError())
            {
                return Operation(std::function<void(CalcStacks &)>({}));
            }
            for (size_t i{2}; i <= num_args; ++i)
            {
                auto t = stacks.typesBack();
                if (t != Operands::Type::scalar)
                {
                    std::string msg = fmt::format("Function \"{}\" cannot take both array and scalar arguments",
                                                  function.value);
                    report.setMessage(msg);
                    return Operation(std::function<void(CalcStacks &)>({}));
                }
                stacks.popType();
            }
        }
        else
        {
            functions::assertNumberOfArgs(function, 1, num_args, report);
            if (report.isError())
            {
                return Operation(std::function<void(CalcStacks &)>({}));
            }
        }
        stacks.pushType(Operands::Type::scalar);
        if (arg_type == Operands::Type::scalar)
        {
            return Operation(functionsReal::reduceArgs(function.value, num_args));
        }
        return Operation(functionsReal::reduceVec(function.value));
    }

    Operation operatorCompiler::compile(const Token &token, Operands &stacks, MaybeError &report)
    {
        if (token.context.is_infix && operatorsCalc::isBinarySymbol(token.value))
        {
            const std::string key = operatorsCalc::binary::compileArgumentsAndKey(token.value, stacks, report);
            if (report.isError())
            {
                report.setPosition(token.start, token.size);
                return Operation(std::function<void(CalcStacks &)>({}));
            }
            const std::function<void(CalcStacks &)> call_back = operatorsCalc::binary::get(key);
            return Operation(call_back);
        }
        if (token.context.is_prefix && operatorsCalc::isBinarySymbol(token.value))
        {
            // reports error if symbol != MINUS, expression compiler ignores PLUS before
            const Operands::Type t = operatorsCalc::unary::compileArgument(token.value, stacks, report);
            if (report.isError())
            {
                report.setPosition(token.start, token.size);
                return Operation(std::function<void(CalcStacks &)>({}));
            }
            assert(t == Operands::Type::scalar || t == Operands::Type::vector);
            if (t == Operands::Type::scalar)
            {
                return Operation(std::function<void(CalcStacks &)>(operatorsCalc::unary::scMinus));
            }
            return Operation(std::function<void(CalcStacks &)>(operatorsCalc::unary::vecMinus));
        }
        throw std::runtime_error("Unknown operator");
    }

}