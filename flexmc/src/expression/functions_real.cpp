#include <cassert>
#include <stdexcept>
#include <format>
#include <algorithm>
#include <numeric>

#include "functions_real.h"

namespace flexMC {

	const Operands::Type functionsReal::compileArgType(const Operands::Type& arg_type) {
		if ((arg_type == Operands::Type::date) || (arg_type == Operands::Type::dateList)) {
			auto msg = std::format(
				"Expected argument type {} or {}, got {}",
				Operands::type2Str(Operands::Type::scalar), 
				Operands::type2Str(Operands::Type::vector), 
				Operands::type2Str(arg_type)
			);
			throw std::runtime_error(msg);
		}
		return arg_type;
	}

	const std::function<void(CalcStacks&)> functionsReal::scalarFunc(
		const std::string& symbol,
		const Operands::Type& return_type) {

		using namespace std::placeholders;

		if ((symbol == EXP) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateScalar, _1,
				[](const double& val) { return std::exp(val); })
			);
		}
		if ((symbol == EXP) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateVector, _1,
				[](const double& val) { return std::exp(val); })
			);
		}
		if ((symbol == LOG) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateScalar, _1,
				[](const double& val) { return std::log(val); })
			);
		}
		if ((symbol == LOG) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateVector, _1,
				[](const double& val) { return std::log(val); })
			);
		}
		if ((symbol == ABS) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateScalar, _1,
				[](const double& val) { return std::fabs(val); })
			);
		}
		if ((symbol == ABS) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateVector, _1,
				[](const double& val) { return std::fabs(val); })
			);
		}
		if ((symbol == SQRT) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateScalar, _1,
				[](const double& val) { return std::sqrt(val); })
			);
		}
		if ((symbol == SQRT) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateVector, _1,
				[](const double& val) { return std::sqrt(val); })
			);
		}
		if ((symbol == SQUARE) && (return_type == Operands::Type::scalar)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateScalar, _1,
				[](const double& val) { return val * val; })
			);
		}
		if ((symbol == SQUARE) && (return_type == Operands::Type::vector)) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::calculateVector, _1,
				[](const double& val) { return val * val; })
			);
		}
		throw std::runtime_error("Unknown function symbol");
	}

	const std::function<void(CalcStacks&)> functionsReal::reduceVec(const std::string& symbol) {

		using namespace std::placeholders;

		if (symbol == SUM) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::accumulateVector, _1,
				[](const double& left, const double& right) { return left + right; },
				0.0)
			);
		}
		if (symbol == PROD) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::accumulateVector, _1,
				[](const double& left, const double& right) { return left * right; },
				1.0)
			);
		}
		if (symbol == MAX) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::maxVector, _1)
			);
		}
		if (symbol == MIN) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::minVector, _1)
			);
		}
		if (symbol == ARGMAX) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::argMaxVector, _1)
			);
		}
		if (symbol == ARGMIN) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::argMinVector, _1)
			);
		}
		if (symbol == LEN) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::lenVector, _1)
			);
		}
		throw std::runtime_error("Unknown function symbol");
	}

	const std::function<void(CalcStacks&)> functionsReal::reduceArgs(const std::string& symbol, const int& size) {

		using namespace std::placeholders;

		if (symbol == SUM) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::accumulateScalars, _1,
				[](const double& left, const double& right) { return left + right; },
				0.0,
				size)
			);
		}
		if (symbol == PROD) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::accumulateScalars, _1,
				[](const double& left, const double& right) { return left * right; },
				1.0,
				size)
			);
		}
		if (symbol == MAX) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::minMaxScalars, _1,
				[](const double& left, const double& right) { return (left > right) ? true : false; },
				size)
			);
		}
		if (symbol == MIN) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::minMaxScalars, _1,
				[](const double& left, const double& right) { return (left < right) ? true : false; },
				size)
			);
		}
		if (symbol == ARGMAX) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::argMaxScalars, _1,
				size)
			);
		}
		if (symbol == ARGMIN) {
			return std::function<void(CalcStacks&)>(std::bind(
				functionsReal::detail::argMinScalars, _1,
				size)
			);
		}
		throw std::runtime_error("Unknown function symbol");
	}

	void functionsReal::detail::calculateScalar(CalcStacks& stacks, double (*call_back) (const double&)) {
		assert(stacks.size(Operands::Type::scalar) > 0);
		const double res = call_back(stacks.scalarsBack());
		stacks.popScalar();
		stacks.pushScalar(res);
	}

	void functionsReal::detail::calculateVector(CalcStacks& stacks, double (*call_back) (const double&)) {
		assert(stacks.size(Operands::Type::vector) > 0);
		std::vector<double>& back = stacks.vectorsBack();
		std::transform(back.cbegin(), back.cend(), back.begin(), call_back);
	}

	void functionsReal::detail::accumulateScalars(CalcStacks& stacks, double (*call_back) (const double&, const double&), const double& init, const int& size) {
		assert(stacks.size(Operands::Type::scalar) >= size);
		double init_ = init;
		for (int i = 0; i < size; ++i) {
			init_ = call_back(std::move(init_), stacks.scalarsBack());
			stacks.popScalar();
		}
		stacks.pushScalar(init_);
	}

	void functionsReal::detail::accumulateVector(CalcStacks& stacks, double (*call_back) (const double&, const double&), const double& init) {
		assert(stacks.size(Operands::Type::vector) > 0);
		std::vector<double>& back = stacks.vectorsBack();
		double res = std::accumulate(back.cbegin(), back.cend(), init, call_back);
		stacks.popVector();
		stacks.pushScalar(res);
	}

	void functionsReal::detail::minMaxScalars(CalcStacks& stacks, bool (*call_back) (const double&, const double&), const int& size) {
		assert(stacks.size(Operands::Type::scalar) >= size);
		double found = stacks.scalarsBack();
		stacks.popScalar();
		for (int i = 1; i < size; ++i) {
			double next = stacks.scalarsBack();
			if (call_back(next, found)) {
				found = next;
			}
			stacks.popScalar();
		}
		stacks.pushScalar(found);
	}

	void functionsReal::detail::argMaxScalars(CalcStacks& stacks, const int& size) {
		assert(stacks.size(Operands::Type::scalar) >= size);
		// operands incoming in reverse order
		std::vector<double> temp(stacks.scalarsEnd() - size, stacks.scalarsEnd());
		int64_t res = std::distance(temp.cbegin(), std::max_element(temp.cbegin(), temp.cend()));
		for (int i = 0; i < size; ++i) { stacks.popScalar(); }
		stacks.pushScalar(static_cast<double>(res));
	}

	void functionsReal::detail::argMinScalars(CalcStacks& stacks, const int& size) {
		assert(stacks.size(Operands::Type::scalar) >= size);
		std::vector<double> temp(stacks.scalarsEnd() - size, stacks.scalarsEnd());
		int64_t res = std::distance(temp.cbegin(), std::min_element(temp.cbegin(), temp.cend()));
		for (int i = 0; i < size; ++i) { stacks.popScalar(); }
		stacks.pushScalar(static_cast<double>(res));
	}

	void functionsReal::detail::maxVector(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::vector) >= 0);
		std::vector<double>& back = stacks.vectorsBack();
		auto res = std::max_element(back.cbegin(), back.cend());
		stacks.pushScalar(*res);
		stacks.popVector();
	}

	void functionsReal::detail::minVector(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::vector) >= 0);
		std::vector<double>& back = stacks.vectorsBack();
		auto res = std::min_element(back.cbegin(), back.cend());
		stacks.pushScalar(*res);
		stacks.popVector();
	}

	void functionsReal::detail::argMaxVector(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::vector) >= 0);
		std::vector<double>& back = stacks.vectorsBack();
		int64_t res = std::distance(back.cbegin(), std::max_element(back.cbegin(), back.cend()));
		stacks.pushScalar(static_cast<double>(res));
		stacks.popVector();
	}

	void functionsReal::detail::argMinVector(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::vector) >= 0);
		std::vector<double>& back = stacks.vectorsBack();
		int64_t res = std::distance(back.cbegin(), std::min_element(back.cbegin(), back.cend()));
		stacks.pushScalar(static_cast<double>(res));
		stacks.popVector();
	}

	void functionsReal::detail::lenVector(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::vector) >= 0);
		std::vector<double>& back = stacks.vectorsBack();
		int64_t res = std::distance(back.cbegin(), back.cend());
		stacks.pushScalar(static_cast<double>(res));
		stacks.popVector();
	}

}