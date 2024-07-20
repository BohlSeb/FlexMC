#pragma once

#include <vector>
#include <string>

#include "tokens.h"
#include "calc_types.h"

namespace flexMC
{

    class Operands
    {

    public:

        [[nodiscard]] bool haveCompiled() const
        { return ((types_.size() == 1) && (functions_.empty())); }

        void pushType(const CType &type);

        void popType();

        void pushArray(const CType &type, const std::size_t &size);

        [[nodiscard]] CType typesBack() const
        { return types_.back(); }

        [[nodiscard]] std::size_t sizesBack() const
        { return vector_sizes_.back(); }

        void pushFunc(const Token &token)
        { functions_.push_back(token); }

        [[nodiscard]] Token funcsBack() const
        { return functions_.back(); }

        void popFunc()
        { functions_.pop_back(); }

        [[nodiscard]] std::size_t tSize() const
        { return types_.size(); }

        [[nodiscard]] std::size_t fSize() const
        { return functions_.size(); }

        [[nodiscard]] std::size_t maxSize(const CType &type) const;

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

        CalcStacks(const std::size_t &s_size, const std::size_t &v_size, const std::size_t &d_size, const std::size_t &d_l_size);

        bool ready() const;

        std::size_t size(const CType &type) const;

        inline void pushScalar(const double &value)
        { scalars_.push_back(value); }

        inline void pushVector(const std::vector<double> &value)
        { vectors_.push_back(value); }

        inline const double &scalarsBack() const
        { return scalars_.back(); }

        inline const std::vector<double> &vectorsBack() const
        { return vectors_.back(); }

        inline std::vector<double> &vectorsBack()
        { return vectors_.back(); }

        inline std::vector<double> &vectorsBeforeBack()
        { return vectors_.end()[-2]; }

        inline const std::vector<double> &vectorsBeforeBack() const
        { return vectors_.end()[-2]; }

        inline void popScalar()
        { scalars_.pop_back(); }

        inline void popVector()
        { vectors_.pop_back(); }

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

        std::vector<int> dates_;

        std::vector<std::vector<int>> date_lists_;

    };

    class PostFixItem
    {

    public:

        virtual ~PostFixItem() = default;

        virtual void evaluate(CalcStacks & /* stacks */)
        { /* evaluate operands on the stack */ }

    };

}