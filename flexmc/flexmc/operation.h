#pragma once

#include <vector>

#include "terminals.h"
#include "tokens.h"
#include "operands.h"


// Todo: We implement one big class now to experiment with 
// allocating it to the heap during calc (this might be bad)


namespace flexMC {


	class Operation {

	public:

		void applyOperator(Operands& stack, const Token& op);

		void applyFunc(Operands& stack, const Token& func);

	private:

		void applyUnary(Operands& stack, const std::string& symbol);

		void applyBinary(Operands& stack, const std::string& symbol);

	};

}