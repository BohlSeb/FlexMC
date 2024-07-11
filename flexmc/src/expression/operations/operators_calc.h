#pragma once

#include <cassert>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cmath>

#include "utils.h"
#include "terminals.h"
#include "language_error.h"
#include "expression_stacks.h"


namespace flexMC::operatorsCalc
{


    bool isBinarySymbol(const std::string &symbol);

    namespace unary
    {

        Operands::Type compileArgument(const std::string &symbol, const Operands &stacks, MaybeError &report);

        void scMinus(CalcStacks &stacks);

        void vecMinus(CalcStacks &stacks);

    }

    namespace binary
    {
        const std::vector<std::string> symbols{
                flexMC::PLUS,
                flexMC::MINUS,
                flexMC::MUL,
                flexMC::DIV,
                flexMC::POW,
        };

        std::string compileArgumentsAndKey(const std::string &symbol, Operands &stacks, MaybeError &report);

        std::function<void(CalcStacks &)> get(const std::string &key);

        std::string makeKey(const std::string &symbol, const Operands::Type &left_t, const Operands::Type &right_t);

        template<class binary_operator>
        void scSc(CalcStacks &stacks, const binary_operator f)
        {
            const double right = stacks.scalarsBack();
            stacks.popScalar();
            const double left = stacks.scalarsBack();
            stacks.popScalar();
            stacks.pushScalar(f(left, right));
        }

        template<class binary_operator>
        void scVec(CalcStacks &stacks, const binary_operator f)
        {
            std::vector<double> &right = stacks.vectorsBack();
            const double left = stacks.scalarsBack();
            stacks.popScalar();
            std::ranges::transform(
                    right,
                    right.begin(),
                    [f, left](auto &right_)
                    { return f(left, right_); }
            );
        }

        template<class binary_operator>
        void vecSc(CalcStacks &stacks, const binary_operator f)
        {
            const double right = stacks.scalarsBack();
            std::vector<double> &left = stacks.vectorsBack();
            stacks.popScalar();
            std::ranges::transform(
                    left,
                    left.begin(),
                    [f, right](auto &left_)
                    { return f(left_, right); }
            );
        }

        template<class binary_operator>
        void vecVec(CalcStacks &stacks, binary_operator f)
        {
            assert(stacks.size(Operands::Type::vector) >= 2);
            const std::vector<double> &right = stacks.vectorsBack();
            std::vector<double> &left = stacks.vectorsBeforeBack();
            std::ranges::transform(
                    left,
                    right,
                    left.begin(),
                    [f](auto &left_, auto &right_)
                    { return f(left_, right_); }
            );
            stacks.popVector();
        }

        using
        enum Operands::Type;

        const std::unordered_map<std::string, std::function<void(CalcStacks &)>, SHash, std::equal_to<>> operators = {
                {
                        makeKey(flexMC::MINUS, scalar, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left - right; }](CalcStacks &stacks)
                        { binary::scSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MINUS, scalar, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left - right; }](CalcStacks &stacks)
                        { binary::scVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MINUS, vector, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left - right; }](CalcStacks &stacks)
                        { binary::vecSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MINUS, vector, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left - right; }](CalcStacks &stacks)
                        { binary::vecVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::PLUS, scalar, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left + right; }](CalcStacks &stacks)
                        { binary::scSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::PLUS, scalar, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left + right; }](CalcStacks &stacks)
                        { binary::scVec(stacks, capture_0); }

                },
                {
                        makeKey(flexMC::PLUS, vector, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left + right; }](CalcStacks &stacks)
                        { binary::vecSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::PLUS, vector, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left + right; }](CalcStacks &stacks)
                        { binary::vecVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MUL, scalar, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left * right; }](CalcStacks &stacks)
                        { binary::scSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MUL, scalar, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left * right; }](CalcStacks &stacks)
                        { binary::scVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MUL, vector, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left * right; }](CalcStacks &stacks)
                        { binary::vecSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::MUL, vector, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left * right; }](CalcStacks &stacks)
                        { binary::vecVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::DIV, scalar, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left / right; }](CalcStacks &stacks)
                        { binary::scSc(stacks, capture_0); }},
                {
                        makeKey(flexMC::DIV, scalar, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left / right; }](CalcStacks &stacks)
                        { binary::scVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::DIV, vector, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return left / right; }](CalcStacks &stacks)
                        { binary::vecSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::DIV, vector, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return left / right; }](CalcStacks &stacks)
                        { binary::vecVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::POW, scalar, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return std::pow(left, right); }](CalcStacks &stacks)
                        { binary::scSc(stacks, capture_0); }},
                {
                        makeKey(flexMC::POW, scalar, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return std::pow(left, right); }](CalcStacks &stacks)
                        { binary::scVec(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::POW, vector, scalar),
                        [capture_0 = [](const double &left, const double &right)
                        { return std::pow(left, right); }](CalcStacks &stacks)
                        { binary::vecSc(stacks, capture_0); }
                },
                {
                        makeKey(flexMC::POW, vector, vector),
                        [capture_0 = [](const double &left, const double &right)
                        { return std::pow(left, right); }](CalcStacks &stacks)
                        { binary::vecVec(stacks, capture_0); }
                }
        };

    }


}