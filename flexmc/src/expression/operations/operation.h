#pragma once

#include <functional>

#include "tokens.h"
#include "language_error.h"
#include "expression_stacks.h"

namespace flexMC
{

    namespace functions
    {

        void assertNumberOfArgs(
                const Token& function,
                const size_t &expected,
                const std::size_t &num_args,
                MaybeError& report);

        void assertNumberOfArgs(
                const Token& function,
                const size_t &min_args,
                const size_t &max_args,
                const std::size_t &num_args,
                MaybeError& report);

    }

    class Operation final : public PostFixItem
    {

    public:

        explicit Operation(const std::function<void(CalcStacks &stacks)> &call_back) : call_back_(call_back)
        {}

        inline void evaluate(CalcStacks &stacks) override
        { call_back_(stacks); }

    private:

        const std::function<void(CalcStacks &stacks)> call_back_;

    };

}
