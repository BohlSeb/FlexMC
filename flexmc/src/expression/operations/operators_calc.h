#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <cmath>

#include "terminals.h"
#include "language_error.h"
#include "expression_stacks.h"


namespace flexMC
{

    namespace operatorsCalc
    {

        bool isBinarySymbol(const std::string &symbol);

        namespace unary
        {

            Operands::Type compileArgument(const std::string &symbol, Operands &stacks, MaybeError &report);

            void scMinus(CalcStacks &stacks);

            void vecMinus(CalcStacks &stacks);

        }

        namespace binary
        {

            using namespace std::placeholders;
            using oprnd_t = Operands::Type;

            const std::vector<std::string> symbols{
                    flexMC::PLUS,
                    flexMC::MINUS,
                    flexMC::MUL,
                    flexMC::DIV,
                    flexMC::POW,
            };

            std::string compileArgumentsAndKey(const std::string &symbol, Operands &stacks, MaybeError &report);

            std::function<void(CalcStacks &)> get(const std::string &key);

            std::string makeKey(const std::string &symbol, const oprnd_t &left_t, const oprnd_t &right_t);

            void scSc(CalcStacks &, double (*call_back)(const double &, const double &));

            void scVec(CalcStacks &, double (*call_back)(const double &, const double &));

            void vecSc(CalcStacks &, double (*call_back)(const double &, const double &));

            void vecVec(CalcStacks &, double (*call_back)(const double &, const double &));

            const std::unordered_map<std::string, std::function<void(CalcStacks &)>> operators = {
                    {
                            makeKey(flexMC::MINUS, oprnd_t::scalar, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left - right; }](auto &&stacks)
                            { binary::scSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MINUS, oprnd_t::scalar, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left - right; }](auto &&stacks)
                            { binary::scVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MINUS, oprnd_t::vector, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left - right; }](auto &&stacks)
                            { binary::vecSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MINUS, oprnd_t::vector, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left - right; }](auto &&stacks)
                            { binary::vecVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::PLUS, oprnd_t::scalar, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left + right; }](auto &&stacks)
                            { binary::scSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::PLUS, oprnd_t::scalar, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left + right; }](auto &&stacks)
                            { binary::scVec(std::forward<decltype(stacks)>(stacks), capture_0); }

                    },
                    {
                            makeKey(flexMC::PLUS, oprnd_t::vector, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left + right; }](auto &&stacks)
                            { binary::vecSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::PLUS, oprnd_t::vector, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left + right; }](auto &&stacks)
                            { binary::vecVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MUL, oprnd_t::scalar, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left * right; }](auto &&stacks)
                            { binary::scSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MUL, oprnd_t::scalar, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left * right; }](auto &&stacks)
                            { binary::scVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MUL, oprnd_t::vector, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left * right; }](auto &&stacks)
                            { binary::vecSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::MUL, oprnd_t::vector, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left * right; }](auto &&stacks)
                            { binary::vecVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::DIV, oprnd_t::scalar, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left / right; }](auto &&stacks)
                            { binary::scSc(std::forward<decltype(stacks)>(stacks), capture_0); }},
                    {
                            makeKey(flexMC::DIV, oprnd_t::scalar, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left / right; }](auto &&stacks)
                            { binary::scVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::DIV, oprnd_t::vector, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return left / right; }](auto &&stacks)
                            { binary::vecSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::DIV, oprnd_t::vector, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return left / right; }](auto &&stacks)
                            { binary::vecVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::POW, oprnd_t::scalar, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return std::pow(left, right); }](auto &&stacks)
                            { binary::scSc(std::forward<decltype(stacks)>(stacks), capture_0); }},
                    {
                            makeKey(flexMC::POW, oprnd_t::scalar, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return std::pow(left, right); }](auto &&stacks)
                            { binary::scVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::POW, oprnd_t::vector, oprnd_t::scalar),
                            [capture_0 = [](const double &left, const double &right)
                            { return std::pow(left, right); }](auto &&stacks)
                            { binary::vecSc(std::forward<decltype(stacks)>(stacks), capture_0); }
                    },
                    {
                            makeKey(flexMC::POW, oprnd_t::vector, oprnd_t::vector),
                            [capture_0 = [](const double &left, const double &right)
                            { return std::pow(left, right); }](auto &&stacks)
                            { binary::vecVec(std::forward<decltype(stacks)>(stacks), capture_0); }
                    }
            };

        }

    }

}