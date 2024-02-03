#include <vector>
#include <cassert>
#include "operands.h"


namespace flexMC {

	void Operands::pushValue(const double& value) {
		values_.push_back(value);
		types_.push_back('v');
		++size_;
	}

	void Operands::pushList(const std::vector<double>& values) {
		lists_.push_back(values);
		types_.push_back('l');
		++size_;
	}

	const double Operands::popValue() {
		assert(typeAsked_);
		double value = values_.back();
		values_.pop_back();
		types_.pop_back();
		typeAsked_ = false;
		--size_;
		return value;
	}

	const std::vector<double> Operands::popList() {
		assert(typeAsked_);
		std::vector<double> values = lists_.back();
		lists_.pop_back();
		types_.pop_back();
		typeAsked_ = false;
		--size_;
		return values;
	}

	const bool Operands::nextIsValue() {
		return checkType('v');
	}

	const bool Operands::nextIsList() {
		return checkType('l');
	}

	bool Operands::checkType(char type) {
		typeAsked_ = true;
		if (!size_) { return false; }
		char next = types_.back();
		if (next == type) { return true; }
		return false;
	}

	const unsigned int Operands::size() { return size_; }

}
