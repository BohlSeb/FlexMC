#include <cassert>
#include <stdexcept>
#include "operand.h"


namespace flexMC
{

    double compileNumber(const Token& token, Operands &stacks, MaybeError &report)
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
            stacks.pushType(Operands::Type::scalar);
        }
        return value_;
    }

    Operands::Type compileVector(const size_t &num_args, Operands &stacks, MaybeError &report)
    {
        assert(num_args > 0);
        assert(stacks.tSize() >= num_args);
        using enum Operands::Type;
        const Operands::Type last_t = stacks.typesBack();
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

    void Number::evaluate(CalcStacks &stacks)
    { stacks.pushScalar(value_); }

    void Vector::evaluate(CalcStacks &stacks)
    {
        assert(stacks.size(Operands::Type::scalar) >= size_);
        std::vector<double> res(stacks.scalarsEnd() - size_, stacks.scalarsEnd());
        for (size_t i{0}; i < size_; ++i)
        {
            stacks.popScalar();
        }
        stacks.pushVector(res);
    }

}
