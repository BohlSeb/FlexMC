#pragma once

#include <vector>
#include <utility>

#include "tokens.h"
#include "calc_types.h"
#include "language_error.h"
#include "static_variables.h"
#include "expression_stacks.h"


namespace flexMC {

    class Expression {

    public:

        void push_back(const Operation &operation) { items_.push_back(operation); }

        void operator()(CalcStacks &stacks) const;

    private:

        std::vector<Operation> items_;

    };

    struct CompileReport {
        CompileReport(const CType &ret_t,
                      const std::size_t &max_s,
                      const std::size_t &max_v) : ret_type(ret_t), max_scalar(max_s), max_vector(max_v) {}

        const CType ret_type;

        const std::size_t max_scalar;

        const std::size_t max_vector;

    };

    // coupling this function with StaticVStorage??
    std::pair<MaybeError, CompileReport>
    compileExpression(const std::vector<Token> &post_fix, Expression &expression, StaticVStorage &storage);

}