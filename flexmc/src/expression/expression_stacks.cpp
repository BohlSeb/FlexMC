#include <cassert>

#include "expression_stacks.h"

namespace flexMC
{

    void Operands::pushType(const CType &type)
    {
        using
        enum CType;
        assert((type == scalar) || (type == date));
        if (type == scalar)
        {
            ++scalar_size_;
            scalar_size_max_ = std::max<size_t>(scalar_size_, scalar_size_max_);
        }
        types_.push_back(type);
    }

    void Operands::pushArray(const CType &type, const std::size_t &size)
    {
        using
        enum CType;
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
        enum CType;
        assert(!types_.empty());
        CType back = types_.back();
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

    std::size_t Operands::maxSize(const CType &type) const
    {
        switch (type)
        {
            case CType::scalar:
                return scalar_size_max_;
            case CType::vector:
                return vec_size_max_;
            default:
                return 0;
        }

    }

    CalcStacks::CalcStacks(const std::size_t &s_size, const std::size_t &v_size, const std::size_t &d_size, const std::size_t &d_l_size)
    {
        scalars_.reserve(s_size);
        vectors_.reserve(v_size);
        dates_.reserve(d_size);
        date_lists_.reserve(d_l_size);
    }


    std::size_t CalcStacks::size(const CType &type) const
    {
        using
        enum CType;
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
        enum CType;
        std::size_t not_ready{0};
        not_ready += size(scalar);
        not_ready += size(vector);
        not_ready += size(date);
        not_ready += size(dateList);
        return (not_ready == 0);
    }

}