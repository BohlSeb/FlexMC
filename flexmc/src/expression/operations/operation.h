#pragma once

#include <functional>

#include "expression_stacks.h"

namespace flexMC
{

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
