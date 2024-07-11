#pragma once

#include <vector>

#include "tokens.h"

namespace flexMC
{

    class Operands
    {

    public:

        enum class Type
        {
            scalar,
            date,
            vector,
            dateList,
            undefined
        };

        [[nodiscard]] bool haveCompiled() const
        { return ((types_.size() == 1) && (functions_.empty())); }

        void pushType(const Type &type);

        void popType();

        void pushArray(const Type &type, const size_t &size);

        [[nodiscard]] Type typesBack() const
        { return types_.back(); }

        [[nodiscard]] size_t sizesBack() const
        { return vector_sizes_.back(); }

        void pushFunc(const Token &token)
        { functions_.push_back(token); }

        [[nodiscard]] Token funcsBack() const
        { return functions_.back(); }

        void popFunc()
        { functions_.pop_back(); }

        [[nodiscard]] size_t tSize() const
        { return types_.size(); }

        [[nodiscard]] size_t fSize() const
        { return functions_.size(); }

        [[nodiscard]] size_t maxSize(const Type &type) const;

        static std::string type2Str(const Type &type);

    private:

        std::vector<Type> types_;

        std::vector<Token> functions_;

        std::vector<size_t> vector_sizes_;

        size_t vec_size_{0};

        size_t vec_size_max_{0};

        size_t scalar_size_{0};

        size_t scalar_size_max_{0};

    };

    class CalcStacks
    {

    public:

        CalcStacks(const size_t &s_size, const size_t &v_size, const size_t &d_size, const size_t &d_l_size);

        bool ready() const;

        size_t size(Operands::Type type) const;

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