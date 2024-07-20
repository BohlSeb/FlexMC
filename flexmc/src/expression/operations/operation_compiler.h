#pragma once

#include "tokens.h"
#include "calc_types.h"
#include "language_error.h"
#include "expression_stacks.h"
#include "operation.h"

namespace flexMC
{

    namespace functionCompiler
    {

        Operation compile(const std::size_t &num_args, Operands &stacks, MaybeError &report);

        namespace detail
        {

            Operation compileScalar(const Token &token,
                                    const std::size_t &num_args,
                                    const Operands &stacks,
                                    MaybeError &report);

            Operation compileReduce(const Token &token,
                                    const std::size_t &num_args,
                                    Operands &stacks,
                                    MaybeError &report);

        }

        void assertNumberOfArgs(
                const Token &function,
                const std::size_t &expected,
                const std::size_t &num_args,
                const CType &arg_type,
                MaybeError &report);

        void assertNumberOfArgs(
                const Token &function,
                const std::size_t &min_args,
                const std::size_t &max_args,
                const std::size_t &num_args,
                const CType &arg_type,
                MaybeError &report);

    }

    namespace operatorCompiler
    {
        Operation compile(const Token &token, Operands &stacks, MaybeError &report);
    }

}