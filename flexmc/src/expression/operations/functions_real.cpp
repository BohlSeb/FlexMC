#include <cassert>
#include <algorithm>
#include <fmt/format.h>

#include "functions_real.h"

#include "iostream"

namespace flexMC::functionsReal
{

    CType
    compileArgType(const Token &token, const CType &arg_type, MaybeError &report)
    {
        using
        enum CType;
        if ((arg_type == date) || (arg_type == dateList))
        {
            auto msg = fmt::format(
                    R"(Function "{}" expects argument type {} or {}, got {})",
                    token.value,
                    cType2Str(scalar),
                    cType2Str(vector),
                    cType2Str(arg_type)
            );
            report.setError(msg, token.start, token.size);
            return undefined;
        }
        return arg_type;
    }

    std::string scalar::makeKey(const std::string &symbol, const CType &arg_type)
    { return symbol + "_" + cType2Str(arg_type); }

    std::function<void(CalcStacks &)> scalar::get(const std::string_view key)
    {
        const auto look_up = functionsReal::scalar::functions.find(key);
        assert(look_up != functions.end());
        return look_up->second;
    }

    std::function<void(CalcStacks &)> reduceVector::get(const std::string_view key)
    {
        const auto look_up = reduceVector::functions.find(key);
        assert(look_up != functions.end());
        return look_up->second;
    }

    std::function<void(CalcStacks &)> reduceArguments::get(const std::string_view key, const std::size_t &size)
    {
        const auto look_up = reduceArguments::functions.find(key);
        assert(look_up != reduceArguments::functions.end());
        const std::function<void(CalcStacks &, const std::size_t &)> f = look_up->second;
        return [f, size](CalcStacks &stacks)
        { return f(stacks, size); };
    }

    void reduceVector::max(CalcStacks &stacks)
    {
        assert(stacks.size(CType::vector) > 0);
        const auto it = std::ranges::max_element(stacks.vectorsBack());
        stacks.pushScalar(*it);
        stacks.popVector();
    }

    void reduceVector::min(CalcStacks &stacks)
    {
        int* array = new int[100];
        delete[] array;
        std::cout << array[0] << std::endl;  // Use after free
        assert(stacks.size(CType::vector) > 0);
        const auto it = std::ranges::min_element(stacks.vectorsBack());
        stacks.pushScalar(*it);
        stacks.popVector();
    }

    void reduceVector::argmax(CalcStacks &stacks)
    {
        assert(stacks.size(CType::vector) > 0);
        const std::vector<double> &back = stacks.vectorsBack();
        const auto res = std::ranges::distance(back.cbegin(), std::ranges::max_element(back));
        stacks.pushScalar(static_cast<double>(res));
        stacks.popVector();
    }

    void reduceVector::argmin(CalcStacks &stacks)
    {
        assert(stacks.size(CType::vector) > 0);
        const std::vector<double> &back = stacks.vectorsBack();
        const auto res = std::ranges::distance(back.cbegin(), std::ranges::min_element(back));
        stacks.pushScalar(static_cast<double>(res));
        stacks.popVector();
    }

    void reduceVector::length(CalcStacks &stacks)
    {
        assert(stacks.size(CType::vector) > 0);
        const auto res = stacks.vectorsBack().size();
        stacks.pushScalar(static_cast<double>(res));
        stacks.popVector();
    }

    void reduceArguments::argMaxScalars(CalcStacks &stacks, const std::size_t &size)
    {
        assert(stacks.size(CType::scalar) >= size);
        const std::vector<double>::const_iterator end = stacks.scalarsEnd();
        const std::vector<double>::const_iterator begin = end - size;
        const auto res = std::distance(begin, std::max_element(begin, end));
        for (size_t i{0}; i < size; ++i)
        { stacks.popScalar(); }
        stacks.pushScalar(static_cast<double>(res));
    }

    void reduceArguments::argMinScalars(CalcStacks &stacks, const std::size_t &size)
    {
        assert((size > 0) && (stacks.size(CType::scalar) >= size));
        const std::vector<double> temp(stacks.scalarsEnd() - size, stacks.scalarsEnd());
        const auto res = std::distance(temp.cbegin(), std::ranges::min_element(temp));
        for (size_t i{0}; i < size; ++i)
        { stacks.popScalar(); }
        stacks.pushScalar(static_cast<double>(res));
    }

}