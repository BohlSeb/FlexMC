#pragma once

#include <functional>
#include <numeric>

#include "tokens.h"
#include "calc_types.h"
#include "language_error.h"
#include "expression_stacks.h"

// #include <iostream>


namespace flexMC::functionsReal
{

    CType compileArgType(const Token &token, const CType &arg_type, MaybeError &report);

    const std::vector<std::string> symbols_scalar{
        EXP,
        LOG,
        ABS,
        SQRT,
        SQUARE,
    };

    const std::vector<std::string> symbols_reduce{
        SUM,
        PROD,
        MAX,
        MIN,
        ARGMAX,
        ARGMIN,
        LEN,
    };

    namespace scalar
    {

        std::function<void(CalcStacks &)> get(const std::string_view key);

        std::string makeKey(const std::string &symbol, const CType &arg_type);

        using
        enum CType;

        template<class scalar_function>
        void calculateScalar(CalcStacks &stacks, scalar_function f)
        {
            assert(stacks.size(scalar) > 0);
            stacks.scalars().back() = f(stacks.scalars().back());
        }

        template<class scalar_function>
        void calculateVector(CalcStacks &stacks, scalar_function f)
        {
            assert(stacks.vectorSizes().size() > 0);
            const std::size_t s = stacks.vectorSizes().back();
            assert(stacks.vectors().size() >= s);
            const auto end = stacks.vectors().end();
            const auto begin = end - s;
            std::transform(begin, end, begin, f);
        }

        const StringMap<std::function<void(CalcStacks &)>> functions{
            {
                makeKey(flexMC::EXP, scalar),
                [capture0 = [](const double &val) { return std::exp(val); }](CalcStacks &stacks) {
                    calculateScalar(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::EXP, vector),
                [capture0 = [](const double &val) { return std::exp(val); }](CalcStacks &stacks) {
                    calculateVector(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::LOG, scalar),
                [capture0 = [](const double &val) { return std::log(val); }](CalcStacks &stacks) {
                    calculateScalar(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::LOG, vector),
                [capture0 = [](const double &val) { return std::log(val); }](CalcStacks &stacks) {
                    calculateVector(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::ABS, scalar),
                [capture0 = [](const double &val) { return std::fabs(val); }](CalcStacks &stacks) {
                    calculateScalar(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::ABS, vector),
                [capture0 = [](const double &val) { return std::fabs(val); }](CalcStacks &stacks) {
                    calculateVector(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::SQRT, scalar),
                [capture0 = [](const double &val) { return std::sqrt(val); }](CalcStacks &stacks) {
                    calculateScalar(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::SQRT, vector),
                [capture0 = [](const double &val) { return std::sqrt(val); }](CalcStacks &stacks) {
                    calculateVector(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::SQUARE, scalar),
                [capture0 = [](const double &val) { return val * val; }](CalcStacks &stacks) {
                    calculateScalar(stacks, capture0);
                }
            },
            {
                makeKey(flexMC::SQUARE, vector),
                [capture0 = [](const double &val) { return val * val; }](CalcStacks &stacks) {
                    calculateVector(stacks, capture0);
                }
            },
        };

    }

    namespace reduceVector
    {

        std::function<void(CalcStacks &)> get(const std::string_view key);

        void max(CalcStacks &stacks);

        void min(CalcStacks &stacks);

        void argmax(CalcStacks &stacks);

        void argmin(CalcStacks &stacks);

        void length(CalcStacks &stacks);

        template<class binary_function>
        void accumulateVector(CalcStacks &stacks,
                              binary_function f,
                              double init)
        {
            assert(!stacks.vectorSizes().empty());
            const std::size_t s = stacks.vectorSizes().back();
            assert(stacks.vectors().size() >= s);
            const auto end = stacks.vectors().end();
            const auto begin = end - s;
            const double res = std::accumulate(begin, end, init, f);
            stacks.vectors().erase(begin, end);
            stacks.vectorSizes().pop_back();
            stacks.scalars().push_back(res);
        }

        const StringMap<std::function<void(CalcStacks &)>> functions{
            {
                flexMC::SUM,
                [capture0 = [](const double &left, const double &right) { return left + right; }](
                    CalcStacks &stacks) { accumulateVector(stacks, capture0, 0.0); }
            },
            {
                flexMC::PROD,
                [capture0 = [](const double &left, const double &right) { return left * right; }](
                    CalcStacks &stacks) { accumulateVector(stacks, capture0, 1.0); }
            },
            {
                flexMC::MAX,
                [](CalcStacks &stacks) { max(stacks); }
            },
            {
                flexMC::MIN,
                [](CalcStacks &stacks) { min(stacks); }
            },
            {
                flexMC::ARGMAX,
                [](CalcStacks &stacks) { argmax(stacks); }
            },
            {
                flexMC::ARGMIN,
                [](CalcStacks &stacks) { argmin(stacks); }
            },
            {
                flexMC::LEN,
                [](CalcStacks &stacks) { length(stacks); }
            },

        };

    }

    namespace reduceArguments
    {

        std::function<void(CalcStacks &)> get(const std::string_view key, const std::size_t &size);

        void argMaxScalars(CalcStacks &stacks, const std::size_t &size);

        void argMinScalars(CalcStacks &stacks, const std::size_t &size);

        template<class binary_function>
        void accumulate(CalcStacks &stacks,
                        binary_function f,
                        double init,
                        const std::size_t &size)
        {
            assert(stacks.size(CType::scalar) >= size);
            for (size_t i{0}; i < size; ++i)
            {
                init = f(init, stacks.scalars().back());
                stacks.scalars().pop_back();
            }
            stacks.scalars().push_back(init);
        }

        template<class binary_function>
        void minmax(CalcStacks &stacks,
                    binary_function f,
                    const std::size_t &size)
        {
            assert(stacks.size(CType::scalar) >= size);
            double found = stacks.scalars().back();
            stacks.scalars().pop_back();
            for (size_t i{1}; i < size; ++i)
            {
                const double next = stacks.scalars().back();
                if (f(next, found))
                {
                    found = next;
                }
                stacks.scalars().pop_back();
            }
            stacks.scalars().push_back(found);
        }

        const StringMap<std::function<void(CalcStacks &, const std::size_t &size)>> functions{
            {
                flexMC::SUM,
                [capture0 = [](const double &left, const double &right) { return left + right; }](CalcStacks &stacks,
                                                                                                  const std::size_t &size) {
                    accumulate(stacks, capture0, 0.0, size);
                }
            },
            {
                flexMC::PROD,
                [capture0 = [](const double &left, const double &right) { return left * right; }](CalcStacks &stacks,
                                                                                                  const std::size_t &size) {
                    accumulate(stacks, capture0, 1.0, size);
                }
            },
            {
                flexMC::MAX,
                [capture0 = [](const double &left, const double &right) { return left > right ? true : false; }](
                    CalcStacks &stacks, const std::size_t &size) { minmax(stacks, capture0, size); }
            },
            {
                flexMC::MIN,
                [capture0 = [](const double &left, const double &right) { return left < right ? true : false; }](
                    CalcStacks &stacks, const std::size_t &size) { minmax(stacks, capture0, size); }
            },
            {
                flexMC::ARGMAX,
                [](CalcStacks &stacks, const size_t &size) { argMaxScalars(stacks, size); }
            },
            {
                flexMC::ARGMIN,
                [](CalcStacks &stacks, const size_t &size) { argMinScalars(stacks, size); }
            },

        };

    }

}


