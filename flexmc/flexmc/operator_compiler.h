#pragma once

#include "operation.h"
// #include "functions_real.h"

namespace flexMC {

	namespace operatorCompiler {

		Operation compile(const std::string& symbol, Operands& stacks) {}

		namespace binary {

			Operation compileBinary(const std::string& symbol, Operands& stacks) {}

		}

	}

}