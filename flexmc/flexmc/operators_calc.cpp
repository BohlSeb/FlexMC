#include <cassert>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "operators_calc.h"
	
namespace flexMC{

	std::function<void(CalcStacks&)> operatorsCalc::compileBinary(const std::string& symbol, Operands& stacks) {
		const std::string key = operatorsCalc::binary::compileArguments(symbol, stacks);
		auto look_up = operatorsCalc::binary::operators.find(key);
		assert(!(look_up == operatorsCalc::binary::operators.end()));
		return look_up->second;
	}

	const bool operatorsCalc::isBinarySymbol(const std::string& symbol) {
		auto look_up = std::find(
			operatorsCalc::binary::symbols.cbegin(), 
			operatorsCalc::binary::symbols.cend(), 
			symbol
		);
		return look_up != operatorsCalc::binary::symbols.end();
	}

	std::string operatorsCalc::binary::compileArguments(const std::string& symbol, Operands& stacks) {
		assert(stacks.tSize() >= 2);
		const oprnd_t right_t = stacks.typesBack();
		stacks.popType();
		const oprnd_t left_t = stacks.typesBack();
		stacks.popType();
		if ((left_t == oprnd_t::date) || (left_t == oprnd_t::dateList)) {
			std::stringstream msg;
			msg << "Binary (left/right) operator " << std::quoted(symbol) << " does not support left operand type: ";
			msg << left_t;
			throw std::runtime_error(msg.str());
		}
		if ((right_t == oprnd_t::date) || (right_t == oprnd_t::dateList)) {
			std::stringstream msg;
			msg << "Binary (left/right) operator " << std::quoted(symbol) << " does not support right operand type: ";
			msg << right_t;
			throw std::runtime_error(msg.str());
		}
		if ((right_t == oprnd_t::vector) && ((symbol == flexMC::DIV) || (symbol == flexMC::POW))) {
			std::stringstream msg;
			msg << "Binary (left/right) operator " << std::quoted(symbol) << " does not support right operand type: ";
			msg << right_t;
			throw std::runtime_error(msg.str());
		}
		if ((left_t == oprnd_t::vector) || (right_t == oprnd_t::vector)) {
			stacks.pushType(oprnd_t::vector);
		}
		else {
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

}