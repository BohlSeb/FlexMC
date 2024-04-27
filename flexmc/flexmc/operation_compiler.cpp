#include <cassert>
#include <algorithm>
#include "operation_compiler.h"
#include "functions_real.h"
#include "operators_calc.h"

namespace flexMC {

	Operation functionCompiler::compile(const int& num_args, Operands& stacks) {
		if (stacks.fSize() <= 0) {
			throw std::runtime_error("Function call compiler expected a function symbol operand in the stack");
		}
		return functionCompiler::detail::compileScalar(num_args, stacks);
	}

	Operation functionCompiler::detail::compileScalar(const int& num_args, Operands& stacks) {
		if (stacks.tSize() < num_args) {
			throw std::runtime_error("Function call compiler did not find enough argument operands in the stack");
		}
		functionsScalar::assertNumberOfArgs(num_args);
		const std::string symbol = stacks.funcsBack();
		stacks.popFunc();
		const Operands::Type return_type = functionsScalar::compileReturnType(stacks.typesBack());
		const std::function<void(CalcStacks&)> call_back = functionsScalar::getCallback(symbol, return_type);
		return Operation(call_back);
	}

	const Operands::Type arrayCompiler::compile(const int& num_args, Operands& stacks) {
		if (num_args < 0) {
			throw std::runtime_error("Array compiler exptected at least one argument from the parser");
		}
		if (stacks.tSize() < num_args) {
			throw std::runtime_error("Array compiler did not find enough operands in the stack");
		}
		const Operands::Type last_t = stacks.typesBack();
		if ((last_t == Operands::Type::dateList) || (last_t == Operands::Type::vector)) {
			throw std::runtime_error("Array cannot not contain another array");
		}
		stacks.popType();
		for (int i = 1; i < num_args; ++i) {
			Operands::Type left = stacks.typesBack();
			if (left != last_t) {
				throw std::runtime_error("All elements of an array must be of the same type");
			}
			stacks.popType();
		}
		stacks.pushType(Operands::Type::vector);
		return last_t;
	}

	Operation operatorCompiler::compile(const Token& token, Operands& stacks) {
		if (token.context.is_infix && operatorsCalc::isBinarySymbol(token.value)) {
			const std::function<void(CalcStacks&)> call_back = operatorsCalc::compileBinary(token.value, stacks);
			return Operation(call_back);
		}
		throw std::runtime_error("Unknown operator");
	}

}