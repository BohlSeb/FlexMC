#pragma once

#include <vector>


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
            dateList
        };

        [[nodiscard]] bool haveCompiled() const
        { return ((types_.size() == 1) && (function_symbols_.empty())); }

        void pushType(const Type &type);

        void popType();

        void pushArray(const Type &type, const size_t &size);

        [[nodiscard]] Type typesBack() const
        { return types_.back(); }

        [[nodiscard]] size_t sizesBack() const
        { return vector_sizes_.back(); }

        void pushFunc(const std::string &symbol)
        { function_symbols_.push_back(symbol); }

        [[nodiscard]] std::string funcsBack() const
        { return function_symbols_.back(); }

        void popFunc()
        { function_symbols_.pop_back(); }

        [[nodiscard]] size_t tSize() const
        { return types_.size(); }

        [[nodiscard]] size_t fSize() const
        { return function_symbols_.size(); }

        [[nodiscard]] size_t maxSize(const Type &type) const;

        static std::string type2Str(const Type &type);

    private:

        std::vector<Type> types_;

        std::vector<std::string> function_symbols_;

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

        [[nodiscard]] bool ready() const;

        void pushScalar(const double &value)
        { scalars_.push_back(value); }

        void pushVector(const std::vector<double> &value)
        { vectors_.push_back(value); }

        void pushDate(const int &value)
        { dates_.push_back(value); }

        void pushDateList(const std::vector<int> &value)
        { date_lists_.push_back(value); }

        [[nodiscard]] const double &scalarsBack() const
        { return scalars_.back(); }

        std::vector<double> &vectorsBack()
        { return vectors_.back(); }

        std::vector<double> &vectorsBeforeBack()
        { return vectors_.end()[-2]; }

        [[nodiscard]] const int &datesBack() const
        { return dates_.back(); }

        std::vector<int> &dateListsBack()
        { return date_lists_.back(); }

        void popScalar()
        { scalars_.pop_back(); }

        void popVector()
        { vectors_.pop_back(); }

        void popDate()
        { dates_.pop_back(); };

        void popDateList()
        { date_lists_.pop_back(); }

        std::vector<double>::const_iterator scalarsStart()
        { return scalars_.cbegin(); }

        std::vector<double>::const_iterator scalarsEnd()
        { return scalars_.cend(); }

        //const bool hasCalculated() const;

        //const bool hasReturnType(const Operands::Type& expected) const;

        // for debugging?
        [[nodiscard]]  size_t size(Operands::Type type) const;

    private:

        std::vector<double> scalars_;

        std::vector<std::vector<double>> vectors_;

        std::vector<int> dates_;

        std::vector<std::vector<int>> date_lists_;

    };

    class PostFixItem
    {

    public:

        virtual void evaluate(CalcStacks & /* stacks */) {}

    };

}