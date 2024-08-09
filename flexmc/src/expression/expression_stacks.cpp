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
            scalar_size_max_ = std::max<std::size_t>(scalar_size_, scalar_size_max_);
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
            vec_size_max_ = std::max<std::size_t>(vec_size_, vec_size_max_);
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

    CalcStacks::CalcStacks(const std::size_t &s_size, const std::size_t &v_size, const std::size_t &d_size,
                           const std::size_t &d_l_size)
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
        not_ready += scalars_.size();
        not_ready += vectors_.size();
        not_ready += dates_.size();
        not_ready += date_lists_.size();
        not_ready += v_sizes_.size();
        not_ready += d_l_sizes_.size();
        return not_ready == 0;
    }

    void CalcStacks::pushVector(const std::vector<SCALAR> &value)
    {
        for (const auto &v: value)
        {
            vectors_.push_back(v);
        }
        v_sizes_.push_back(value.size());
    }

    void CalcStacks::pushDateList(const std::vector<DATE> &value)
    {
        for (const auto &v: value)
        {
            date_lists_.push_back(v);
        }
        d_l_sizes_.push_back(value.size());
    }

    std::vector<SCALAR> CalcStacks::vectorResult() const
    {
        assert(v_sizes_.size() == 1);
        const std::size_t s = v_sizes_.back();
        assert(vectors_.size() == s);
        return {vectors_.end() - s, vectors_.end()};
    }

    std::vector<DATE> CalcStacks::dateListResult() const
    {
        assert(d_l_sizes_.size() == 1);
        const std::size_t s = d_l_sizes_.back();
        assert(date_lists_.size() == s);
        return {date_lists_.end() - s, date_lists_.end()};
    }

    void CalcStacks::popVectorResult()
    {

        assert(vectorSizes().size() == 1);
        assert(vectors().size() == vectorSizes().back());
        vectors_.erase(vectors_.end() - v_sizes_.back(), vectors_.end());
        v_sizes_.pop_back();
    }

    void CalcStacks::popDateListResult()
    {
        assert(d_l_sizes_.size() == 1);
        assert(date_lists_.size() == d_l_sizes_.back());
        date_lists_.erase(date_lists_.end() - d_l_sizes_.back(), date_lists_.end());
        d_l_sizes_.pop_back();
    }

}
