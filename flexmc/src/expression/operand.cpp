#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include "operand.h"
#include "calc_types.h"


namespace flexMC
{

    double compileNumber(const Token &token, Operands &stacks, MaybeError &report)
    {
        double value_{1.0};
        try
        {
            value_ = std::stod(token.value);
        } catch (const std::invalid_argument &error)
        {
            report.setError(error.what(), token.start, token.size);
        }
        if (!report.isError())
        {
            stacks.pushType(CType::scalar);
        }
        return value_;
    }

    Operation compileNumberOperation(const double &value)
    {
        return Operation([value](CalcStacks &stacks)
                         { stacks.scalars().emplace_back(value); });
    }

    CType compileVector(const std::size_t &num_args, Operands &stacks, MaybeError &report)
    {
        assert(num_args > 0);
        assert(stacks.tSize() >= num_args);
        using
        enum CType;
        const CType last_t = stacks.typesBack();
        if ((last_t == dateList) || (last_t == vector))
        {
            report.setMessage("List cannot not contain another list (Matrices not allowed)");
            return undefined;
        }
        stacks.popType();
        for (size_t i{1}; i < num_args; ++i)
        {
            if (stacks.typesBack() != last_t)
            {
                report.setMessage("All elements of a list must be of the same type");
                return undefined;
            }
            stacks.popType();
        }
        stacks.pushArray(vector, num_args);
        return last_t;
    }

    Operation compileVectorOperation(const std::size_t &size)
    {
        const VectorAppend v{size};
        return Operation([v](CalcStacks &stacks)
                         { v(stacks); });
    }

    void VectorAppend::operator()(CalcStacks &stacks) const
    {
        assert(stacks.scalars().size() >= size_);
        // bottleneck / bug?:
        // we have the stack of the form stack([x, y, z, append_callback(3)]) which should result in [x, y, y] so the
        // callback cannot pop_back() the values that it is done using.
        // possible fix:
        // making CalcStacks::vectors_ a std::deque instead of a std::vector, replacing push_back by
        // push_front etc (this would also replace the slightly awkward iterators end = end(); begin = end - size;
        // everywhere by the (faster?) begin = begin(); end = begin + size;)
        // On the other hand, vector/dateList variables would have to be pushed back in reverse order.
        // current fix:
        // iterating twice below, not introducing std::deque
        const auto end = stacks.scalars().end();
        const auto begin = end - size_;
        std::move(begin, end, std::back_inserter(stacks.vectors()));
        stacks.scalars().erase(begin, end);
        stacks.vectorSizes().push_back(size_);
    }

}
