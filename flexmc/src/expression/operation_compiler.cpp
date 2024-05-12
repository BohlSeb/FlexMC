#include <cassert>
#include <algorithm>

#include "functions_real.h"
#include "operators_calc.h"
#include "operation_compiler.h"

namespace flexMC {

	Operation functionCompiler::compile(const int& num_args, Operands& stacks) {
		if (stacks.fSize() <= 0) {
			throw std::runtime_error("Function call compiler expected a function symbol operand in the stack");
		}
		const std::string symbol = stacks.funcsBack();
		stacks.popFunc();
		using namespace functionsReal;
		if (std::find(symbols_scalar.cbegin(), symbols_scalar.cend(), symbol) != symbols_scalar.cend()) {
			return functionCompiler::detail::compileScalar(symbol, num_args, stacks);
		}
		if (std::find(symbols_reduce.cbegin(), symbols_reduce.cend(), symbol) != symbols_reduce.cend()) {
			return functionCompiler::detail::compileReduce(symbol, num_args, stacks);
		}
		else {
			throw std::runtime_error("Unknown function symbol");
		}
	}

	Operation functionCompiler::detail::compileScalar(const std::string& symbol, const int& num_args, Operands& stacks) {
		if (stacks.tSize() < num_args) {
			throw std::runtime_error("Function call compiler did not find enough argument operands in the stack");
		}
		functions::assertNumberOfArgs(1, 1, num_args);
		const Operands::Type return_type = functionsReal::compileArgType(stacks.typesBack());
		const std::function<void(CalcStacks&)> call_back = functionsReal::scalarFunc(symbol, return_type);
		return Operation(call_back);
	}

	Operation functionCompiler::detail::compileReduce(const std::string& symbol, const int& num_args, Operands& stacks) {
		if (stacks.tSize() < num_args) {
			throw std::runtime_error("Function call compiler did not find enough argument operands in the stack");
		}
		const Operands::Type arg_type = functionsReal::compileArgType(stacks.typesBack());
		stacks.popType();
		if (arg_type == Operands::Type::scalar) {
			functions::assertNumberOfArgs(2, -1, num_args);
			for (int i = 2; i <= num_args; ++i) {
				auto t = stacks.typesBack();
				if (t != Operands::Type::scalar) {
					throw std::runtime_error("Function cannot take both list and scalar arguments at the same time");
				}
				stacks.popType();
			}
		}
		else { 
			functions::assertNumberOfArgs(1, 1, num_args);
		}
		stacks.pushType(Operands::Type::scalar);
		if (arg_type == Operands::Type::scalar) {
			return Operation(functionsReal::reduceArgs(symbol, num_args));
		}
		return Operation(functionsReal::reduceVec(symbol));
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
		stacks.pushArray(Operands::Type::vector, num_args);
		return last_t;
	}

	Operation operatorCompiler::compile(const Token& token, Operands& stacks) {
		if (token.context.is_infix && operatorsCalc::isBinarySymbol(token.value)) {
			const std::function<void(CalcStacks&)> call_back = operatorsCalc::compileBinary(token.value, stacks);
			return Operation(call_back);
		}
		if (token.context.is_prefix && operatorsCalc::isBinarySymbol(token.value)) {
			// raises, including if symbol != MINUS, expression compiler ignores PLUS before
			const Operands::Type t = operatorsCalc::unary::compileArgument(token.value, stacks);
			assert(t == Operands::Type::scalar || t == Operands::Type::vector);
			if (t == Operands::Type::scalar) {
				return Operation(std::function<void(CalcStacks&)>(operatorsCalc::unary::scMinus));
			}
			return Operation(std::function<void(CalcStacks&)>(operatorsCalc::unary::vecMinus));
		}
		throw std::runtime_error("Unknown operator");
	}

}