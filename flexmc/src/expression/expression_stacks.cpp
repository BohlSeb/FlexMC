#include <cassert>
#include <string>

#include "expression_stacks.h"

namespace flexMC
{

    void Operands::pushType(const Type &type)
    {
        assert((type == Type::scalar) || (type == Type::date));
        if (type == Type::scalar)
        {
            ++scalar_size_;
            scalar_size_max_ = std::max<size_t>(scalar_size_, scalar_size_max_);
        }
        types_.push_back(type);
    }

    void Operands::pushArray(const Type &type, const size_t &size)
    {
        assert((type == Type::vector) || (type == Type::dateList));
        types_.push_back(type);
        vector_sizes_.push_back(size);
        if (type == Type::vector)
        {
            ++vec_size_;
            vec_size_max_ = std::max<size_t>(vec_size_, vec_size_max_);
        }
    }

    void Operands::popType()
    {
        assert(!types_.empty());
        Type back = types_.back();
        if ((back == Type::vector) || (back == Type::dateList))
        {
            assert(!vector_sizes_.empty());
            vector_sizes_.pop_back();
        }
        switch (back)
        {
            case Type::scalar:
                --scalar_size_;
                break;
            case Type::vector:
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
        switch (type)
        {
            case Type::scalar:
                return "Scalar";
            case Type::vector:
                return "Vector";
            case Type::date:
                return "Date";
            case Type::dateList:
                return "DateList";
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
        switch (type)
        {
            case Operands::Type::scalar:
                return scalars_.size();
            case Operands::Type::date:
                return dates_.size();
            case Operands::Type::vector:
                return vectors_.size();
            case Operands::Type::dateList:
                return date_lists_.size();
            default:
                return 0;
        }
    }

    // Refactor
    bool CalcStacks::ready() const
    {
        size_t not_ready{0};
        not_ready += size(Operands::Type::scalar);
        not_ready += size(Operands::Type::vector);
        not_ready += size(Operands::Type::date);
        not_ready += size(Operands::Type::dateList);
        return (not_ready == 0);
    }

}