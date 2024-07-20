#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "tokens.h"
#include "calc_types.h"
#include "language_error.h"
#include "expression_stacks.h"


namespace flexMC
{

    class Expression
    {

    public:

        template<class T>
        inline void addItem(T item)
        { items_.emplace_back(std::make_unique<T>(std::move(item))); }

        void evaluate(CalcStacks &stacks) const;

    private:

        std::vector<std::unique_ptr<PostFixItem>> items_;

    };

    struct CompileReport
    {
        CompileReport(const CType &ret_t,
                      const std::size_t &max_s,
                      const std::size_t &max_v) : ret_type(ret_t), max_scalar(max_s), max_vector(max_v)
        {}

        const CType ret_type;

        const std::size_t max_scalar;

        const std::size_t max_vector;

    };

    std::pair<MaybeError, CompileReport> compileExpression(const std::vector<Token> &post_fix, Expression &expression);

}