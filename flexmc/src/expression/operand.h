#pragma once

#include <string>

#include "tokens.h"
#include "calc_types.h"
#include "language_error.h"
#include "expression_stacks.h"


// Todo: Keep precision in mind

namespace flexMC
{

    double compileNumber(const Token &token, Operands &stacks, MaybeError &report);

    CType compileVector(const std::size_t &num_args, Operands &stacks, MaybeError &report);

    class Number final : public PostFixItem
    {

    public:

        explicit Number(double value) : value_(value)
        {}

        void evaluate(CalcStacks &stacks) override;

    private:

        const double value_;

    };

    class Vector final : public PostFixItem
    {

    public:

        explicit Vector(const std::size_t &size) : size_(size)
        {}

        void evaluate(CalcStacks &stacks) override;

    private:

        const std::size_t size_;

    };

}
