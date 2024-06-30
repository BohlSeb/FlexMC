#pragma once

#include <string>

#include "tokens.h"
#include "language_error.h"
#include "expression_stacks.h"


// Todo: Keep precision in mind

namespace flexMC
{

    double compileNumber(const Token &token, Operands &stacks, MaybeError &report);

    Operands::Type compileVector(const size_t &num_args, Operands &stacks, MaybeError &report);

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

        explicit Vector(const size_t &size) : size_(size)
        {}

        void evaluate(CalcStacks &stacks) override;

    private:

        const size_t size_;

    };

}
