#include <cassert>
#include <algorithm>
#include <fmt/format.h>

#include "functions_real.h"


namespace flexMC::functionsReal {

    CType
    compileArgType(const Token &token, const CType &arg_type, MaybeError &report) {
        using
        enum CType;
        if ((arg_type == date) || (arg_type == date_list)) {
            auto msg = fmt::format(
                R"(Function "{}" expects argument type {} or {}, got {})",
                token.value,
                cType2Str(scalar),
                cType2Str(vector),
                cType2Str(arg_type)
            );
            report.setError(msg, token);
            return undefined;
        }
        return arg_type;
    }

    std::string scalar::makeKey(const std::string &symbol, const CType &arg_type) {
        return symbol + "_" + cType2Str(arg_type);
    }

    std::function<void(CalcStacks &)> scalar::get(const std::string_view key) {
        const auto look_up = functionsReal::scalar::FUNCTIONS.find(key);
        assert(look_up != FUNCTIONS.end());
        return look_up->second;
    }

    std::function<void(CalcStacks &)> reduceVector::get(const std::string_view key) {
        const auto look_up = reduceVector::FUNCTIONS.find(key);
        assert(look_up != FUNCTIONS.end());
        return look_up->second;
    }

    std::function<void(CalcStacks &)> reduceArguments::get(const std::string_view key, const std::size_t &size) {
        const auto look_up = reduceArguments::FUNCTIONS.find(key);
        assert(look_up != reduceArguments::FUNCTIONS.end());
        const std::function<void(CalcStacks &, const std::size_t &)> f = look_up->second;
        return [f, size](CalcStacks &stacks) { return f(stacks, size); };
    }

    void reduceVector::max(CalcStacks &stacks) {
        assert(!stacks.vectorSizes().empty());
        const std::size_t s = stacks.vectorSizes().back();
        assert(stacks.vectors().size() >= s);
        const auto end = stacks.vectors().end();
        const auto begin = end - s;
        const auto it = std::max_element(begin, end);
        stacks.scalars().push_back(*it);
        stacks.vectors().erase(begin, end);
        stacks.vectorSizes().pop_back();
    }

    void reduceVector::min(CalcStacks &stacks) {
        assert(!stacks.vectorSizes().empty());
        const std::size_t s = stacks.vectorSizes().back();
        assert(stacks.vectors().size() >= s);
        const auto end = stacks.vectors().end();
        const auto begin = end - s;
        const auto it = std::min_element(begin, end);
        stacks.scalars().push_back(*it);
        stacks.vectors().erase(begin, end);
        stacks.vectorSizes().pop_back();
    }

    void reduceVector::argmax(CalcStacks &stacks) {
        assert(!stacks.vectorSizes().empty());
        const std::size_t s = stacks.vectorSizes().back();
        assert(stacks.vectors().size() >= s);
        const auto end = stacks.vectors().end();
        const auto begin = end - s;
        const auto res = std::distance(begin, std::max_element(begin, end));
        stacks.scalars().push_back(static_cast<double>(res));
        stacks.vectors().erase(begin, end);
        stacks.vectorSizes().pop_back();
    }

    void reduceVector::argmin(CalcStacks &stacks) {
        assert(!stacks.vectorSizes().empty());
        const std::size_t s = stacks.vectorSizes().back();
        assert(stacks.vectors().size() >= s);
        const auto end = stacks.vectors().end();
        const auto begin = end - s;
        const auto res = std::distance(begin, std::min_element(begin, end));
        stacks.scalars().push_back(static_cast<double>(res));
        stacks.vectors().erase(begin, end);
        stacks.vectorSizes().pop_back();
    }

    void reduceVector::length(CalcStacks &stacks) {
        assert(!stacks.vectorSizes().empty());
        const std::size_t s = stacks.vectorSizes().back();
        assert(stacks.vectors().size() >= s);
        stacks.scalars().push_back(static_cast<double>(s));
        stacks.vectors().erase(stacks.vectors().end() - s, stacks.vectors().end());
        stacks.vectorSizes().pop_back();
    }

    void reduceArguments::argMaxScalars(CalcStacks &stacks, const std::size_t &size) {
        assert(stacks.size(CType::scalar) >= size);
        const auto end = stacks.scalars().end();
        const auto begin = end - size;
        const auto res = std::distance(begin, std::max_element(begin, end));
        stacks.scalars().erase(begin, end);
        stacks.scalars().push_back(static_cast<double>(res));
    }

    void reduceArguments::argMinScalars(CalcStacks &stacks, const std::size_t &size) {
        assert((size > 0) && (stacks.size(CType::scalar) >= size));
        const auto end = stacks.scalars().end();
        const auto begin = end - size;
        const auto res = std::distance(begin, std::min_element(begin, end));
        stacks.scalars().erase(begin, end);
        stacks.scalars().push_back(static_cast<double>(res));
    }

}