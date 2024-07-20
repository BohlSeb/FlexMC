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
        const auto is_scalar = std::ranges::find(symbols_scalar, fun.value) != symbols_scalar.cend();
        const auto is_reduce = std::ranges::find(symbols_reduce, fun.value) != symbols_reduce.cend();
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
                                            const Operands &stacks,
                                            MaybeError &report)
    {
        assert(stacks.tSize() >= num_args);
        const CType arg_type = stacks.tSize() > 0 ? stacks.typesBack() : CType::scalar;
        assertNumberOfArgs(function, 1, num_args, arg_type, report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        const CType return_type = functionsReal::compileArgType(function, arg_type, report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        const std::string key = functionsReal::scalar::makeKey(function.value, return_type);
        const std::function<void(CalcStacks &)> call_back = functionsReal::scalar::get(key);
        return Operation(call_back);
    }

    Operation
    functionCompiler::detail::compileReduce(const Token &function,
                                            const size_t &num_args,
                                            Operands &stacks,
                                            MaybeError &report)
    {
        using
        enum CType;
        assert(stacks.tSize() >= num_args);
        const CType dummy_arg_type = stacks.tSize() > 0 ? stacks.typesBack() : scalar;
        assertNumberOfArgs(function, 1, 0, num_args, dummy_arg_type, report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        const CType arg_type = functionsReal::compileArgType(function, stacks.typesBack(), report);
        if (report.isError())
        {
            return Operation(std::function<void(CalcStacks &)>({}));
        }
        stacks.popType();
        if (arg_type == scalar)
        {
            assertNumberOfArgs(function, 2, 0, num_args, arg_type, report);
            if (report.isError())
            {
                return Operation(std::function<void(CalcStacks &)>({}));
            }
            for (size_t i{2}; i <= num_args; ++i)
            {
                if (stacks.typesBack() != scalar)
                {
                    std::string msg = fmt::format(R"(Function "{}" cannot take both <Vector> and <Scalar> arguments)",
                                                  function.value);
                    report.setMessage(msg);
                    return Operation(std::function<void(CalcStacks &)>({}));
                }
                stacks.popType();
            }
        }
        else
        {
            assertNumberOfArgs(function, 1, num_args, arg_type, report);
            if (report.isError())
            {
                return Operation(std::function<void(CalcStacks &)>({}));
            }
        }
        stacks.pushType(scalar);
        if (arg_type == scalar)
        {
            return Operation(functionsReal::reduceArguments::get(function.value, num_args));
        }
        return Operation(functionsReal::reduceVector::get(function.value));
    }

    void functionCompiler::assertNumberOfArgs(const Token &function,
                                              const size_t &expected,
                                              const size_t &num_args,
                                              const CType &arg_type,
                                              MaybeError &report)
    {
        if (expected != num_args)
        {
            auto msg = fmt::format(R"(Function "{}" with argument type <{}> takes exactly {} argument(s), got {})",
                                   function.value, cType2Str(arg_type), expected, num_args);
            report.setError(msg, function.start, function.size);
        }
    }

    void functionCompiler::assertNumberOfArgs(const Token &function,
                                              const size_t &min_args,
                                              const size_t &max_args,
                                              const size_t &num_args,
                                              const CType &arg_type,
                                              MaybeError &report)
    {
        if ((num_args < min_args) || ((max_args > 0) && (num_args > max_args)))
        {
            std::string msg;
            if (max_args > 0)
            {
                msg = fmt::format(
                        R"(Function "{}" with argument type <{}> takes between {} and {} argument(s), got {})",
                        function.value, cType2Str(arg_type), min_args, max_args, num_args
                );
            }
            else
            {
                msg = fmt::format(
                        R"(Function "{}" with argument type <{}> takes at least {} argument(s), got {})",
                        function.value, cType2Str(arg_type), min_args, num_args
                );
            }
            report.setError(msg, function.start, function.size);
        }

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
            const CType t = operatorsCalc::unary::compileArgument(token.value, stacks, report);
            if (report.isError())
            {
                report.setPosition(token.start, token.size);
                return Operation(std::function<void(CalcStacks &)>({}));
            }
            assert(t == CType::scalar || t == CType::vector);
            if (t == CType::scalar)
            {
                return Operation(std::function<void(CalcStacks &)>(operatorsCalc::unary::scMinus));
            }
            return Operation(std::function<void(CalcStacks &)>(operatorsCalc::unary::vecMinus));
        }
        report.setError("Internal Error: Unknown operator", 1, 1);
        return Operation(std::function<void(CalcStacks &)>({}));
    }

}