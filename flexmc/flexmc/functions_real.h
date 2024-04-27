#pragma once
#include <string>
#include <functional>
#include "operation.h"
#include "terminals.h"


namespace flexMC {

	namespace functionsScalar {

		const std::vector<std::string> bin_symbols{
			flexMC::EXP,
			flexMC::LOG,
			flexMC::ABS,
			flexMC::SQRT,
			flexMC::SQUARE,
		};

		void assertNumberOfArgs(const int& num_args);

		const Operands::Type compileReturnType(const Operands::Type& arg_type);

		const std::function<void(CalcStacks&)> getCallback(
			const std::string& symbol, 
			const Operands::Type& return_type);

		namespace detail {

			void calculate(CalcStacks& stacks, double (*call_back) (const double&));

			void calculate_vector(CalcStacks& stacks, double (*call_back) (const double&));

			void exp(CalcStacks& stacks);

			void expVec(CalcStacks& stacks);

			void log(CalcStacks& stacks);

			void logVec(CalcStacks& stacks);

			void abs(CalcStacks& stacks);

			void absVec(CalcStacks& stacks);

			void sqrt(CalcStacks& stacks);

			void sqrtVec(CalcStacks& stacks);

			void square(CalcStacks& stacks);

			void squareVec(CalcStacks& stacks);

		}

	}

}
