#pragma once

#include "expression_stacks.h"
#include "operation.h"
#include "tokens.h"

namespace flexMC
{

    namespace functionCompiler
    {

        Operation compile(const size_t &num_args, Operands &stacks);

        namespace detail
        {

            Operation compileScalar(const std::string &symbol, const size_t &num_args, Operands &stacks);

            Operation compileReduce(const std::string &symbol, const size_t &num_args, Operands &stacks);

        }

    }

    namespace arrayCompiler
    {

        Operands::Type compile(const size_t &num_args, Operands &stacks);

    }

    namespace operatorCompiler
    {

        Operation compile(const Token &token, Operands &stacks);

    }

}