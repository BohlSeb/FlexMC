#include <cassert>
#include <stdexcept>

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
                         { stacks.pushScalar(value); });
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
        assert(stacks.size(CType::scalar) >= size_);
        std::vector<double> res(stacks.scalarsEnd() - size_, stacks.scalarsEnd());
        for (size_t i{0}; i < size_; ++i)
        {
            stacks.popScalar();
        }
        stacks.pushVector(res);
    }

}
