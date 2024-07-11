#include <cassert>
#include <string>

#include "expression_stacks.h"

namespace flexMC
{

    void Operands::pushType(const Type &type)
    {
        using
        enum Type;
        assert((type == scalar) || (type == date));
        if (type == scalar)
        {
            ++scalar_size_;
            scalar_size_max_ = std::max<size_t>(scalar_size_, scalar_size_max_);
        }
        types_.push_back(type);
    }

    void Operands::pushArray(const Type &type, const size_t &size)
    {
        using
        enum Type;
        assert((type == vector) || (type == dateList));
        types_.push_back(type);
        vector_sizes_.push_back(size);
        if (type == vector)
        {
            ++vec_size_;
            vec_size_max_ = std::max<size_t>(vec_size_, vec_size_max_);
        }
    }

    void Operands::popType()
    {
        using
        enum Type;
        assert(!types_.empty());
        Type back = types_.back();
        if ((back == vector) || (back == dateList))
        {
            assert(!vector_sizes_.empty());
            vector_sizes_.pop_back();
        }
        switch (back)
        {
            case scalar:
                --scalar_size_;
                break;
            case vector:
                --vec_size_;
                break;
            default:
                break;
        }
        types_.pop_back();
    }

    size_t Operands::maxSize(const Type &type) const
    {
        switch (type)
        {
            case Type::scalar:
                return scalar_size_max_;
            case Type::vector:
                return vec_size_max_;
            default:
                return 0;
        }

    }

    std::string Operands::type2Str(const Type &type)
    {
        using
        enum Type;
        switch (type)
        {
            case scalar:
                return "Scalar";
            case vector:
                return "Vector";
            case date:
                return "Date";
            case dateList:
                return "DateList";
            case undefined:
                return "undefined";
            default:
                return "";
        }
    }

    CalcStacks::CalcStacks(const size_t &s_size, const size_t &v_size, const size_t &d_size, const size_t &d_l_size)
    {
        scalars_.reserve(s_size);
        vectors_.reserve(v_size);
        dates_.reserve(d_size);
        date_lists_.reserve(d_l_size);
    }


    size_t CalcStacks::size(Operands::Type type) const
    {
        using
        enum Operands::Type;
        switch (type)
        {
            case scalar:
                return scalars_.size();
            case date:
                return dates_.size();
            case vector:
                return vectors_.size();
            case dateList:
                return date_lists_.size();
            default:
                return 0;
        }
    }

    // Refactor
    bool CalcStacks::ready() const
    {
        using
        enum Operands::Type;
        size_t not_ready{0};
        not_ready += size(scalar);
        not_ready += size(vector);
        not_ready += size(date);
        not_ready += size(dateList);
        return (not_ready == 0);
    }

}