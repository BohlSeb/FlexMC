#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "functions_real.h"

namespace flexMC {

	const Operands::Type functionsScalar::compileReturnType(const Operands::Type& arg_type) {
		if ((arg_type == Operands::Type::date) || (arg_type == Operands::Type::dateList)) {
			// std format
			std::stringstream msg;
			msg << "expected argument type: " << Operands::Type::scalar;
			msg << " or " << Operands::Type::vector << ", got " << arg_type;
			throw std::runtime_error(msg.str());
		}
		return arg_type;
	}

	void functionsScalar::assertNumberOfArgs(const int& num_args) {
		functions::assertNumberOfArgs(1, 1, num_args);
	}

	const std::function<void(CalcStacks&)> functionsScalar::getCallback(
		const std::string& symbol, 
		const Operands::Type& return_type) {

		if ((symbol == EXP) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::exp);
		}
		if ((symbol == EXP) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::expVec);
		}
		if ((symbol == LOG) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::log);
		}
		if ((symbol == LOG) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::logVec);
		}
		if ((symbol == ABS) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::abs);
		}
		if ((symbol == ABS) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::absVec);
		}
		if ((symbol == SQRT) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::sqrt);
		}
		if ((symbol == SQRT) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::sqrtVec);
		}
		if ((symbol == SQUARE) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::square);
		}
		if ((symbol == SQUARE) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(functionsScalar::detail::squareVec);
		}
		throw std::runtime_error("Unknown symbol");
	}

	void functionsScalar::detail::exp(CalcStacks& stacks) {
		detail::calculate(stacks, [](const double& val) { return std::exp(val); });
	}

	void functionsScalar::detail::expVec(CalcStacks& stacks) {
		detail::calculate_vector(stacks, [](const double& val) { return std::exp(val); });
	}

	void functionsScalar::detail::log(CalcStacks& stacks) {
		detail::calculate(stacks, [](const double& val) { return std::log(val); });
	}

	void functionsScalar::detail::logVec(CalcStacks& stacks) {
		detail::calculate_vector(stacks, [](const double& val) { return std::log(val); });
	}

	void functionsScalar::detail::abs(CalcStacks& stacks) {
		detail::calculate(stacks, [](const double& val) { return std::fabs(val); });
	}

	void functionsScalar::detail::absVec(CalcStacks& stacks) {
		detail::calculate_vector(stacks, [](const double& val) { return std::fabs(val); });
	}

	void functionsScalar::detail::sqrt(CalcStacks& stacks) {
		detail::calculate(stacks, [](const double& val) { return std::sqrt(val); });
	}

	void functionsScalar::detail::sqrtVec(CalcStacks& stacks) {
		detail::calculate_vector(stacks, [](const double& val) { return std::sqrt(val); });
	}

	void functionsScalar::detail::square(CalcStacks& stacks) {
		detail::calculate(stacks, [](const double& val) { return val * val; });
	}

	void functionsScalar::detail::squareVec(CalcStacks& stacks) {
		detail::calculate_vector(stacks, [](const double& val) { return val * val; });
	}

	void functionsScalar::detail::calculate(CalcStacks& stacks, double (*call_back) (const double&)) {
		const std::size_t n_scalars = stacks.size(Operands::Type::scalar);
		// Should never happen
		if (n_scalars < 1) {
			throw std::runtime_error("Calc: expected double on the stack");
		}
		const double res = call_back(stacks.scalarsBack());
		stacks.popScalar();
		stacks.pushScalar(res);
	}

	void functionsScalar::detail::calculate_vector(CalcStacks& stacks, double (*call_back) (const double&)) {
		const std::size_t n_vectors = stacks.size(Operands::Type::vector);
		// Should never happen
		if (n_vectors < 1) {
			throw std::runtime_error("Calc: expected vector<double> on the stack");
		}
		std::vector<double>& back = stacks.vectorsBack();
		std::transform(back.cbegin(), back.cend(), back.begin(), call_back);
	}

}