#include <cassert>
#include <algorithm>
#include <ranges>
#include <fmt/format.h>

#include "operators_calc.h"

namespace flexMC
{

    bool operatorsCalc::isBinarySymbol(const std::string &symbol)
    {
        auto look_up = std::ranges::find(operatorsCalc::binary::symbols, symbol);
        return look_up != operatorsCalc::binary::symbols.end();
    }

    CType operatorsCalc::unary::compileArgument(const std::string &symbol, const Operands &stacks, MaybeError &report)
    {
        using
        enum CType;
        assert(stacks.tSize() >= 1);
        if (symbol != flexMC::MINUS)
        {
            auto msg = fmt::format(R"(Undefined: trying to compile unary operator for symbol "{}")", symbol);
            report.setMessage(msg);
            return undefined;
        }
        const CType t = stacks.typesBack();
        if ((t == date) || (t == dateList))
        {
            auto msg = fmt::format(R"(Unary operator "{}" does not support operand type: "{}")",
                                   symbol,
                                   cType2Str(t));
            report.setMessage(msg);
            return undefined;
        }
        return t;
    }

    void operatorsCalc::unary::scMinus(CalcStacks &stacks)
    {
        assert(stacks.size(CType::scalar) >= 1);
        auto res = stacks.scalars().back() * -1;
        stacks.scalars().pop_back();
        stacks.scalars().push_back(res);
    }

    void operatorsCalc::unary::vecMinus(CalcStacks &stacks)
    {
        assert(stacks.vectorSizes().size() > 0);
        const std::size_t s = stacks.vectorSizes().back();
        assert(stacks.vectors().size() >= s);
        const auto end = stacks.vectors().end();
        const auto begin = end - s;
        std::transform(begin, end, begin, std::negate<double>());
    }

    std::string
    operatorsCalc::binary::compileArgumentsAndKey(const std::string &symbol, Operands &stacks, MaybeError &report)
    {
        using
        enum CType;
        assert(stacks.tSize() >= 2);
        const CType right_t = stacks.typesBack();
        std::size_t maybe_right_s = 0;
        if ((right_t == vector) || (right_t == dateList))
        {
            maybe_right_s += stacks.sizesBack();
        }
        stacks.popType();
        const CType left_t = stacks.typesBack();

        if ((left_t == date) || (left_t == dateList))
        {
            const std::string t_ = cType2Str(left_t);
            auto msg = fmt::format(R"(Binary operator "{}" does not support left operand type: "{}")", symbol, t_);
            report.setMessage(msg);
            return "";
        }
        if ((right_t == date) || (right_t == dateList))
        {
            const std::string t_ = cType2Str(right_t);
            auto msg = fmt::format(R"(Binary operator "{}" does not support right operand type: "{}")", symbol, t_);
            report.setMessage(msg);
            return "";
        }

        if ((left_t == vector) && (right_t == vector))
        {
            const std::size_t left_s = stacks.sizesBack();
            if (left_s != maybe_right_s)
            {
                auto msg = fmt::format(
                        R"(Binary operator "{}" got vectors of different lengths: {} and {})",
                        symbol,
                        left_s,
                        maybe_right_s
                );
                report.setMessage(msg);
                return "";
            }
            stacks.popType();
            stacks.pushArray(vector, left_s);
        }
        else if (left_t == vector)
        {
            const std::size_t left_s = stacks.sizesBack();
            stacks.popType();
            stacks.pushArray(vector, left_s);
        }
        else if (right_t == vector)
        {
            stacks.popType();
            stacks.pushArray(vector, maybe_right_s);
        }
        else
        {
            stacks.popType();
            stacks.pushType(scalar);
        }
        return makeKey(symbol, left_t, right_t);
    }

    std::string operatorsCalc::binary::makeKey(
            const std::string &symbol,
            const CType &left_t,
            const CType &right_t)
    {
        return symbol + cType2Str(left_t) + cType2Str(right_t);
    }

    std::function<void(CalcStacks &)> operatorsCalc::binary::get(const std::string &key)
    {
        const auto look_up = operatorsCalc::binary::operators.find(key);
        assert(look_up != operatorsCalc::binary::operators.end());
        return look_up->second;
    }

}