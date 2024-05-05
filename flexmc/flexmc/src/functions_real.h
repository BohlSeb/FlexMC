#pragma once

#include <string>
#include <functional>
#include "operation.h"
#include "terminals.h"


namespace flexMC {

	namespace functionsReal {

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

		const Operands::Type compileArgType(const Operands::Type& arg_type);

		const std::function<void(CalcStacks&)> scalarFunc(const std::string& symbol, const Operands::Type& return_type);

		const std::function<void(CalcStacks&)> reduceVec(const std::string& symbol);

		const std::function<void(CalcStacks&)> reduceArgs(const std::string& symbol, const int& num_args);

		namespace detail {

			void calculateScalar(CalcStacks& stacks, double (*call_back) (const double&));

			void calculateVector(CalcStacks& stacks, double (*call_back) (const double&));

			void accumulateScalars(CalcStacks& stacks, double (*call_back) (const double&, const double&), const double& init, const int& size);

			void accumulateVector(CalcStacks& stacks, double (*call_back) (const double&, const double&), const double& init);

			void minMaxScalars(CalcStacks& stacks, bool (*call_back) (const double&, const double&), const int& size);

			void argMaxScalars(CalcStacks& stacks, const int& size);

			void argMinScalars(CalcStacks& stacks, const int& size);

			void maxVector(CalcStacks& stacks);

			void minVector(CalcStacks& stacks);

			void argMaxVector(CalcStacks& stacks);

			void argMinVector(CalcStacks& stacks);

			void lenVector(CalcStacks& stacks);

		}

	}

}
