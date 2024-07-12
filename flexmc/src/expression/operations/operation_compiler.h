#pragma once

#include "tokens.h"
#include "language_error.h"
#include "expression_stacks.h"
#include "operation.h"

namespace flexMC
{

    namespace functionCompiler
    {

        Operation compile(const size_t &num_args, Operands &stacks, MaybeError &report);

        namespace detail
        {

            Operation compileScalar(const Token &token,
                                    const size_t &num_args,
                                    const Operands &stacks,
                                    MaybeError &report);

            Operation compileReduce(const Token &token,
                                    const size_t &num_args,
                                    Operands &stacks,
                                    MaybeError &report);

        }

        void assertNumberOfArgs(
                const Token& function,
                const size_t &expected,
                const std::size_t &num_args,
                const Operands::Type& arg_type,
                MaybeError& report);

        void assertNumberOfArgs(
                const Token& function,
                const size_t &min_args,
                const size_t &max_args,
                const std::size_t &num_args,
                const Operands::Type& arg_type,
                MaybeError& report);

    }

    namespace operatorCompiler
    {
        Operation compile(const Token &token, Operands &stacks, MaybeError &report);
    }

}