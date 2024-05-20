#include <cassert>

#include "operand.h"


namespace flexMC {

	void Number::compile(Operands& stacks) { stacks.pushType(Operands::Type::scalar); }

	void Number::evaluate(CalcStacks& stacks) { stacks.pushScalar(value_); }
	
	void Vector::evaluate(CalcStacks& stacks) {
		assert(stacks.size(Operands::Type::scalar) >= size_);
		std::vector<double> res(stacks.scalarsEnd() - size_, stacks.scalarsEnd());
		for (size_t i{0}; i < size_; ++i) {
			stacks.popScalar();
		}
		stacks.pushVector(res);
	}

}
