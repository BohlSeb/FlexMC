#pragma once

#include <cassert>
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

        bool ready() const;

        std::size_t size(const CType &type) const;

        // refactor this to get closer to vectors()
        inline void pushScalar(const double &value)
        { scalars_.push_back(value); }

        inline void pushVector(const std::vector<double> &value)
        {
            for (const auto &v: value)
            { vecs_2.push_back(v); }
            v_sizes_.push_back(value.size());
        }


        inline const double &scalarsBack() const
        { return scalars_.back(); }

        inline std::vector<double> vectorResult() const
        {
            std::vector<double> out;
            assert(!vectorSizes().empty());
            const std::size_t s = vectorSizes().back();
            assert(vectors().size() >= s);
            const auto end = vectors().end();
            const auto begin = end - s;
            for (VECTOR::const_iterator it = begin; it != end; ++it) {
                out.push_back(*it);
            }
            return out;
        }

        inline void popVector()
        {
            assert(vectorSizes().size() == 1);
            const std::size_t s = vectorSizes().back();
            assert(vectors().size() == s);
            popVector2(s);
            vectorSizes().pop_back(); }

        inline std::vector<double> &vectors()
        { return vecs_2; };

        inline const std::vector<double> &vectors() const
        { return vecs_2; };

        inline std::vector<std::size_t> &vectorSizes()
        { return v_sizes_; };

        inline const std::vector<std::size_t> &vectorSizes() const
        { return v_sizes_; };


//        inline std::vector<double> &vectorsBeforeBack()
//        { return vectors_.end()[-2]; }
//
//        inline const std::vector<double> &vectorsBeforeBack() const
//        { return vectors_.end()[-2]; }

        inline void popScalar()
        { scalars_.pop_back(); }

        inline void popVector2(const std::size_t size)
        {
            for (size_t s{0}; s < size; ++s)
            { vecs_2.pop_back(); }
        }

        //

        inline std::vector<double>::const_iterator scalarsEnd() const
        { return scalars_.cend(); }

        inline void pushDate(const int &value)
        { dates_.push_back(value); }

        inline void pushDateList(const std::vector<int> &value)
        { date_lists_.push_back(value); }

        inline void popDate()
        { dates_.pop_back(); };

        inline void popDateList()
        { date_lists_.pop_back(); }

        [[nodiscard]] inline const int &datesBack() const
        { return dates_.back(); }

        inline std::vector<int> &dateListsBack()
        { return date_lists_.back(); }

    private:

        std::vector<double> scalars_;

        std::vector<std::vector<double>> vectors_;

        std::vector<double> vecs_2;

        std::vector<std::size_t> v_sizes_;

        std::vector<int> dates_;

        std::vector<std::vector<int>> date_lists_;

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