#pragma once

#include "expression_stacks.h"
#include "operation.h"
#include "tokens.h"

namespace flexMC {

	namespace functionCompiler {

		Operation compile(const int& num_args, Operands& stacks);

		namespace detail {

			Operation compileScalar(const std::string& symbol, const int& num_args, Operands& stacks);

			Operation compileReduce(const std::string& symbol, const int& num_args, Operands& stacks);

		}
		
	}

	namespace arrayCompiler {

		const Operands::Type compile(const int& num_args, Operands& stacks);

	}

	namespace operatorCompiler {

		Operation compile(const Token& token, Operands& stacks);

	}

}