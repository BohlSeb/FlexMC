#include <cassert>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <functional>
#include "operators_calc.h"

namespace flexMC {

	const Operands::Type operatorsCalc::unary::compileArgument(const std::string& symbol, Operands& stacks) {
		assert(stacks.tSize() >= 1);
		if (symbol != flexMC::MINUS) {
			std::stringstream msg;
			msg << "Undefined: trying to compile unary operator for symbol " << std::quoted(symbol);
			throw std::runtime_error(msg.str());
		}
		const Operands::Type t = stacks.typesBack();
		if ((t == Operands::Type::date) || (t == Operands::Type::dateList)) {
			std::stringstream msg;
			msg << "Unary operator " << std::quoted(symbol) << " does not support operand type: ";
			msg << t;
			throw std::runtime_error(msg.str());
		}
		return t;
	}

	void operatorsCalc::unary::scMinus(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::scalar) >= 1);
		auto res = stacks.scalarsBack() * -1;
		stacks.popScalar();
		stacks.pushScalar(res);
	}

	void operatorsCalc::unary::vecMinus(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::vector) >= 1);
		std::vector<double>& back = stacks.vectorsBack();
		std::transform(back.cbegin(), back.cend(), back.begin(), std::negate<double>());
	}

	const bool operatorsCalc::isBinarySymbol(const std::string& symbol) {
		auto look_up = std::find(
			operatorsCalc::binary::symbols.cbegin(),
			operatorsCalc::binary::symbols.cend(),
			symbol
		);
		return look_up != operatorsCalc::binary::symbols.end();
	}

	std::function<void(CalcStacks&)> operatorsCalc::compileBinary(const std::string& symbol, Operands& stacks) {
		const std::string key = operatorsCalc::binary::compileArguments(symbol, stacks);
		auto look_up = operatorsCalc::binary::operators.find(key);
		assert(!(look_up == operatorsCalc::binary::operators.end()));
		return look_up->second;
	}

	std::string operatorsCalc::binary::compileArguments(const std::string& symbol, Operands& stacks) {
		assert(stacks.tSize() >= 2);
		const oprnd_t right_t = stacks.typesBack();
		int maybe_right_s = 0;
		if ((right_t == oprnd_t::vector) || (right_t == oprnd_t::dateList)) {
			maybe_right_s += stacks.sizesBack();
		}
		stacks.popType();
		const oprnd_t left_t = stacks.typesBack();

		if ((left_t == oprnd_t::date) || (left_t == oprnd_t::dateList)) {
			std::stringstream msg;
			msg << "Binary operator " << std::quoted(symbol) << " does not support left operand type: ";
			msg << left_t;
			throw std::runtime_error(msg.str());
		}
		if ((right_t == oprnd_t::date) || (right_t == oprnd_t::dateList)) {
			std::stringstream msg;
			msg << "Binary operator " << std::quoted(symbol) << " does not support right operand type: ";
			msg << right_t;
			throw std::runtime_error(msg.str());
		}

		if ((left_t == oprnd_t::vector) && (right_t == oprnd_t::vector)) {
			const int left_s = stacks.sizesBack();
			if (left_s != maybe_right_s) {
				std::stringstream msg;
				msg << "Binaray operator " << std::quoted(symbol);
				msg << " got vectors of different lengths: ";
				msg << left_s << ", " << maybe_right_s;
				throw std::runtime_error(msg.str());
			}
			stacks.popType();
			stacks.pushArray(oprnd_t::vector, left_s);
		}
		else if (left_t == oprnd_t::vector) {
			const int left_s = stacks.sizesBack();
			stacks.popType();
			stacks.pushArray(oprnd_t::vector, left_s);
		}
		else if (right_t == oprnd_t::vector) {
			stacks.popType();
			stacks.pushArray(oprnd_t::vector, maybe_right_s);
		}
		else {
			stacks.popType();
			stacks.pushType(oprnd_t::scalar);
		}
		return makeKey(symbol, left_t, right_t);
	}

	std::string operatorsCalc::binary::makeKey(
		const std::string& symbol,
		const oprnd_t& left_t,
		const oprnd_t& right_t) {
		std::string left = (left_t == oprnd_t::scalar) ? "_S" : "_V";
		std::string right = (right_t == oprnd_t::scalar) ? "_S" : "_V";
		return symbol + left + right;
	}

	void operatorsCalc::binary::scSc(CalcStacks& stacks, double (*call_back) (const double&, const double&)) {
		const double right = stacks.scalarsBack();
		stacks.popScalar();
		const double left = stacks.scalarsBack();
		stacks.popScalar();
		stacks.pushScalar(call_back(left, right));
	}

	void operatorsCalc::binary::scVec(CalcStacks& stacks, double (*call_back) (const double&, const double&)) {
		std::vector<double>& right = stacks.vectorsBack();
		const double left = stacks.scalarsBack();
		stacks.popScalar();
		std::transform(right.cbegin(), right.cend(), right.begin(), std::bind(call_back, left, _1));
	}

	void operatorsCalc::binary::vecSc(CalcStacks& stacks, double (*call_back) (const double&, const double&)) {
		const double right = stacks.scalarsBack();
		std::vector<double>& left = stacks.vectorsBack();
		stacks.popScalar();
		std::transform(left.cbegin(), left.cend(), left.begin(), std::bind(call_back, _1, right));
	};

	void operatorsCalc::binary::vecVec(CalcStacks& stacks, double (*call_back) (const double&, const double&)) {
		assert(stacks.size(oprnd_t::vector) >= 2);
		const std::vector<double>& right = stacks.vectorsBack();
		std::vector<double>& left = stacks.vectorsBeforeBack();
		std::transform(left.cbegin(), left.cend(), right.cbegin(), left.begin(), std::bind(call_back, _1, _2));
		stacks.popVector();
	};

}