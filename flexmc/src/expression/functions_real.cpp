#include <stdexcept>
#include <cassert>
#include <fmt/format.h>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "functions_real.h"

namespace flexMC
{

    Operands::Type functionsReal::compileArgType(const Operands::Type &arg_type)
    {
        if ((arg_type == Operands::Type::date) || (arg_type == Operands::Type::dateList))
        {
            auto msg = fmt::format(
                    "Expected argument type {} or {}, got {}",
                    Operands::type2Str(Operands::Type::scalar),
                    Operands::type2Str(Operands::Type::vector),
                    Operands::type2Str(arg_type)
            );
            throw std::runtime_error(msg);
        }
        return arg_type;
    }

    std::function<void(CalcStacks &)> functionsReal::scalarFunc(
            const std::string &symbol,
            const Operands::Type &return_type)
    {

        if ((symbol == EXP) && (return_type == Operands::Type::scalar))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::exp(val); }](auto &&stacks)
                    { functionsReal::detail::calculateScalar(std::forward<decltype(stacks)>(stacks), capture0); }
            };
        }
        if ((symbol == EXP) && (return_type == Operands::Type::vector))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::exp(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateVector(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == LOG) && (return_type == Operands::Type::scalar))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::log(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateScalar(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == LOG) && (return_type == Operands::Type::vector))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::log(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateVector(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == ABS) && (return_type == Operands::Type::scalar))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::fabs(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateScalar(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == ABS) && (return_type == Operands::Type::vector))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::fabs(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateVector(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == SQRT) && (return_type == Operands::Type::scalar))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::sqrt(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateScalar(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == SQRT) && (return_type == Operands::Type::vector))
        {
            return {
                    [capture0 = [](const double &val)
                    { return std::sqrt(val); }](auto &&stacks)
                    {
                        functionsReal::detail::calculateVector(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == SQUARE) && (return_type == Operands::Type::scalar))
        {
            return {
                    [capture0 = [](const double &val)
                    { return val * val; }](auto &&stacks)
                    {
                        functionsReal::detail::calculateScalar(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        if ((symbol == SQUARE) && (return_type == Operands::Type::vector))
        {
            return {
                    [capture0 = [](const double &val)
                    { return val * val; }](auto &&stacks)
                    {
                        functionsReal::detail::calculateVector(std::forward<decltype(stacks)>(stacks), capture0);
                    }
            };
        }
        throw std::runtime_error("Unknown function symbol");
    }

    std::function<void(CalcStacks &)> functionsReal::reduceVec(const std::string &symbol)
    {

        using namespace std::placeholders;

        if (symbol == SUM)
        {
            return {
                    [capture0 = [](const double &left, const double &right)
                    { return left + right; }](auto &&stacks)
                    {
                        functionsReal::detail::accumulateVector(std::forward<decltype(stacks)>(stacks), capture0, 0.0);
                    }
            };
        }
        if (symbol == PROD)
        {
            return {
                    [capture0 = [](const double &left, const double &right)
                    { return left * right; }](auto &&stacks)
                    {
                        functionsReal::detail::accumulateVector(std::forward<decltype(stacks)>(stacks), capture0, 1.0);
                    }
            };
        }
        if (symbol == MAX)
        {
            return {
                    [](auto &&stacks)
                    {
                        functionsReal::detail::maxVector(std::forward<decltype(stacks)>(stacks));
                    }
            };
        }
        if (symbol == MIN)
        {
            return {
                    [](auto &&stacks)
                    {
                        functionsReal::detail::minVector(std::forward<decltype(stacks)>(stacks));
                    }
            };
        }
        if (symbol == ARGMAX)
        {
            return {
                    [](auto &&stacks)
                    {
                        functionsReal::detail::argMaxVector(std::forward<decltype(stacks)>(stacks));
                    }
            };
        }
        if (symbol == ARGMIN)
        {
            return {
                    [](auto &&stacks)
                    {
                        functionsReal::detail::argMinVector(std::forward<decltype(stacks)>(stacks));
                    }
            };
        }
        if (symbol == LEN)
        {
            return {
                    [](auto &&stacks)
                    {
                        functionsReal::detail::lenVector(std::forward<decltype(stacks)>(stacks));
                    }
            };
        }
        throw std::runtime_error("Unknown function symbol");
    }

    std::function<void(CalcStacks &)> functionsReal::reduceArgs(const std::string &symbol, const size_t &size)
    {

        using namespace functionsReal;

        if (symbol == SUM)
        {
            return {
                    [capture_0 = [](const double &left, const double &right)
                    { return left + right; }, size](auto &&stacks)
                    { detail::accumulateScalars(std::forward<decltype(stacks)>(stacks), capture_0, 0, size); }
            };
        }
        if (symbol == PROD)
        {
            return {
                    [capture_0 = [](const double &left, const double &right)
                    { return left * right; }, size](auto &&stacks)
                    { detail::accumulateScalars(std::forward<decltype(stacks)>(stacks), capture_0, 1, size); }
            };
        }
        if (symbol == MAX)
        {
            return {
                    [capture_0 = [](const double &left, const double &right)
                    { return (left > right) ? true : false; }, size](auto &&stacks)
                    { detail::minMaxScalars(std::forward<decltype(stacks)>(stacks), capture_0, size); }
            };
        }
        if (symbol == MIN)
        {
            return {
                    [capture_0 = [](const double &left, const double &right)
                    { return (left < right) ? true : false; }, size](auto &&stacks)
                    { detail::minMaxScalars(std::forward<decltype(stacks)>(stacks), capture_0, size); }
            };
        }
        if (symbol == ARGMAX)
        {
            return {
                    [size](auto &&stacks)
                    { functionsReal::detail::argMaxScalars(std::forward<decltype(stacks)>(stacks), size); }
            };
        }
        if (symbol == ARGMIN)
        {
            return {
                    [size](auto &&stacks)
                    { functionsReal::detail::argMinScalars(std::forward<decltype(stacks)>(stacks), size); }
            };
        }
        throw std::runtime_error("Unknown function symbol");
    }

    void functionsReal::detail::calculateScalar(CalcStacks &stacks, double (*call_back)(const double &))
    {
        assert(stacks.size(Operands::Type::scalar) > 0);
        const double res = call_back(stacks.scalarsBack());
        stacks.popScalar();
        stacks.pushScalar(res);
    }

    void functionsReal::detail::calculateVector(CalcStacks &stacks, double (*call_back)(const double &))
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        std::transform(back.cbegin(), back.cend(), back.begin(), call_back);
    }

    void
    functionsReal::detail::accumulateScalars(CalcStacks &stacks, double (*call_back)(const double &, const double &),
                                             const double &init, const size_t &size)
    {
        assert(stacks.size(Operands::Type::scalar) >= size);
        double init_ = init;
        for (size_t i{0}; i < size; ++i)
        {
            init_ = call_back(init_, stacks.scalarsBack());
            stacks.popScalar();
        }
        stacks.pushScalar(init_);
    }

    void
    functionsReal::detail::accumulateVector(CalcStacks &stacks, double (*call_back)(const double &, const double &),
                                            const double &init)
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        double res = std::accumulate(back.cbegin(), back.cend(), init, call_back);
        stacks.popVector();
        stacks.pushScalar(res);
    }

    void functionsReal::detail::minMaxScalars(CalcStacks &stacks, bool (*call_back)(const double &, const double &),
                                              const size_t &size)
    {
        assert(stacks.size(Operands::Type::scalar) >= size);
        double found = stacks.scalarsBack();
        stacks.popScalar();
        for (size_t i{1}; i < size; ++i)
        {
            double next = stacks.scalarsBack();
            if (call_back(next, found))
            {
                found = next;
            }
            stacks.popScalar();
        }
        stacks.pushScalar(found);
    }

    void functionsReal::detail::argMaxScalars(CalcStacks &stacks, const size_t &size)
    {
        assert(stacks.size(Operands::Type::scalar) >= size);
        // operands incoming in reverse order
        std::vector<double> temp(stacks.scalarsEnd() - size, stacks.scalarsEnd());
        auto res = std::distance(temp.cbegin(), std::max_element(temp.cbegin(), temp.cend()));
        for (size_t i{0}; i < size; ++i)
        { stacks.popScalar(); }
        stacks.pushScalar(static_cast<double>(res));
    }

    void functionsReal::detail::argMinScalars(CalcStacks &stacks, const size_t &size)
    {
        assert((size > 0) && (stacks.size(Operands::Type::scalar) >= size));
        std::vector<double> temp(stacks.scalarsEnd() - size, stacks.scalarsEnd());
        auto res = std::distance(temp.cbegin(), std::min_element(temp.cbegin(), temp.cend()));
        for (size_t i{0}; i < size; ++i)
        { stacks.popScalar(); }
        stacks.pushScalar(static_cast<double>(res));
    }

    void functionsReal::detail::maxVector(CalcStacks &stacks)
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        auto res = std::max_element(back.cbegin(), back.cend());
        stacks.pushScalar(*res);
        stacks.popVector();
    }

    void functionsReal::detail::minVector(CalcStacks &stacks)
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        auto res = std::min_element(back.cbegin(), back.cend());
        stacks.pushScalar(*res);
        stacks.popVector();
    }

    void functionsReal::detail::argMaxVector(CalcStacks &stacks)
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        auto res = std::distance(back.cbegin(), std::max_element(back.cbegin(), back.cend()));
        stacks.pushScalar(static_cast<double>(res));
        stacks.popVector();
    }

    void functionsReal::detail::argMinVector(CalcStacks &stacks)
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        auto res = std::distance(back.cbegin(), std::min_element(back.cbegin(), back.cend()));
        stacks.pushScalar(static_cast<double>(res));
        stacks.popVector();
    }

    void functionsReal::detail::lenVector(CalcStacks &stacks)
    {
        assert(stacks.size(Operands::Type::vector) > 0);
        std::vector<double> &back = stacks.vectorsBack();
        auto res = std::distance(back.cbegin(), back.cend());
        stacks.pushScalar(static_cast<double>(res));
        stacks.popVector();
    }

}