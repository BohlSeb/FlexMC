#pragma once

#include <functional>

#include "terminals.h"
#include "expression_stacks.h"


namespace flexMC::functionsReal
{

    const std::vector<std::string> symbols_scalar{
            EXP,
            LOG,
            ABS,
            SQRT,
            SQUARE,
    };

    const std::vector<std::string> symbols_reduce{
            SUM,
            PROD,
            MAX,
            MIN,
            ARGMAX,
            ARGMIN,
            LEN,
    };

    Operands::Type compileArgType(const Operands::Type &arg_type);

    std::function<void(CalcStacks &)> scalarFunc(const std::string &symbol, const Operands::Type &return_type);

    std::function<void(CalcStacks &)> reduceVec(const std::string &symbol);

    std::function<void(CalcStacks &)> reduceArgs(const std::string &symbol, const size_t &num_args);

    namespace detail
    {

        void calculateScalar(CalcStacks &stacks, double (*call_back)(const double &));

        void calculateVector(CalcStacks &stacks, double (*call_back)(const double &));

        void accumulateScalars(CalcStacks &stacks,
                               double (*call_back)(const double &, const double &),
                               const double &init,
                               const size_t &size);

        void accumulateVector(CalcStacks &stacks,
                              double (*call_back)(const double &, const double &),
                              const double &init);

        void minMaxScalars(CalcStacks &stacks,
                           bool (*call_back)(const double &, const double &),
                           const size_t &size);

        void argMaxScalars(CalcStacks &stacks, const size_t &size);

        void argMinScalars(CalcStacks &stacks, const size_t &size);

        void maxVector(CalcStacks &stacks);

        void minVector(CalcStacks &stacks);

        void argMaxVector(CalcStacks &stacks);

        void argMinVector(CalcStacks &stacks);

        void lenVector(CalcStacks &stacks);

    }

}


