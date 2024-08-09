#pragma once

#include <vector>
#include <string>
#include <functional>

#include "tokens.h"
#include "calc_types.h"

namespace flexMC
{

    class Operands
    {

    public:

        bool haveCompiled() const
        { return ((types_.size() == 1) && (functions_.empty())); }

        void pushType(const CType &type);

        void popType();

        void pushArray(const CType &type, const std::size_t &size);

        CType typesBack() const
        { return types_.back(); }

        std::size_t sizesBack() const
        { return vector_sizes_.back(); }

        void pushFunc(const Token &token)
        { functions_.push_back(token); }

        Token funcsBack() const
        { return functions_.back(); }

        void popFunc()
        { functions_.pop_back(); }

        std::size_t tSize() const
        { return types_.size(); }

        std::size_t fSize() const
        { return functions_.size(); }

        std::size_t maxSize(const CType &type) const;

    private:

        std::vector<CType> types_;

        std::vector<Token> functions_;

        std::vector<size_t> vector_sizes_;

        std::size_t vec_size_{0};

        std::size_t vec_size_max_{0};

        std::size_t scalar_size_{0};

        std::size_t scalar_size_max_{0};

    };

    class CalcStacks
    {

    public:

        CalcStacks(const std::size_t &s_size, const std::size_t &v_size, const std::size_t &d_size,
                   const std::size_t &d_l_size);

        inline std::vector<SCALAR> &scalars()
        { return scalars_; };

        inline const std::vector<SCALAR> &scalars() const
        { return scalars_; };

        inline std::vector<SCALAR> &vectors()
        { return vectors_; };

        inline const std::vector<SCALAR> &vectors() const
        { return vectors_; };

        inline std::vector<DATE> &dates()
        { return dates_; };

        inline const std::vector<DATE> &dates() const
        { return dates_; };

        inline std::vector<DATE> &datesLists()
        { return date_lists_; };

        inline const std::vector<DATE> &datesLists() const
        { return date_lists_; };

        inline std::vector<std::size_t> &vectorSizes()
        { return v_sizes_; };

        inline const std::vector<std::size_t> &vectorSizes() const
        { return v_sizes_; };

        inline std::vector<std::size_t> &dateListSizes()
        { return d_l_sizes_; };

        inline const std::vector<std::size_t> &dateListSizes() const
        { return d_l_sizes_; };

        void pushVector(const std::vector<SCALAR> &value);

        void pushDateList(const std::vector<DATE> &value);

        std::vector<SCALAR> vectorResult() const;

        std::vector<DATE> dateListResult() const;

        void popVectorResult();

        void popDateListResult();

        bool ready() const;

        std::size_t size(const CType &type) const;

    private:

        std::vector<SCALAR> scalars_;

        std::vector<DATE> dates_;

        std::vector<SCALAR> vectors_;

        std::vector<DATE> date_lists_;

        std::vector<std::size_t> v_sizes_;

        std::vector<std::size_t> d_l_sizes_;

    };

    class Operation
    {

    public:

        // std::function may allocate
        explicit Operation(const std::function<void(CalcStacks &stacks)> &call_back) : call_back_(call_back)
        {}

        void operator()(CalcStacks &stacks) const
        { call_back_(stacks); }

    private:

        const std::function<void(CalcStacks &stacks)> call_back_;

    };

}