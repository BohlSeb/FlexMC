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

    Operation compileNumberOperation(const double &value);

    CType compileVector(const std::size_t &num_args, Operands &stacks, MaybeError &report);

    Operation compileVectorOperation(const std::size_t &size);

    class VectorAppend
    {

    public:

        explicit VectorAppend(const std::size_t &size) : size_(size)
        {}

        void operator()(CalcStacks &stacks) const;

    private:

        const std::size_t size_;

    };

}
