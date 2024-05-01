#include <sstream>
#include "expression_compiler.h"
#include "operation_compiler.h"
#include "operand.h"

namespace flexMC {

	void ExpressionCompiler::compile() {
		Operands operands;
		// Reading backwards
		for (auto tok = post_fix_.rbegin(); tok != post_fix_.rend(); ++tok) {
			const Token::Type t = tok->type;
			if (t == Token::Type::num) {
				Number num = Number(tok->value);
				num.compile(operands);
				result_.push_back(std::make_shared<Number>(std::move(num)));
				continue;
			}
			if (t == Token::Type::fun) {
				operands.pushFunc(tok->value);
				continue;
			}
			if (t == Token::Type::call_) {
				Operation function = functionCompiler::compile(tok->context.num_args, operands);
				result_.push_back(std::make_shared<Operation>(std::move(function)));
				continue;
			}
			if (t == Token::Type::append_) {
				Operands::Type elem_t = arrayCompiler::compile(tok->context.num_args, operands);
				if (elem_t == Operands::Type::scalar) {
					result_.push_back(std::make_shared<Vector>(tok->context.num_args));
				}
				continue;
			}
			if (t == Token::Type::op) {
				Operation op = operatorCompiler::compile(*tok, operands);
				result_.push_back(std::make_shared<Operation>(std::move(op)));
			}
		}
		if (!operands.haveCompiled()) {
			throw std::runtime_error("Could not compile expression up to its return type");
		}
		return_type_ = operands.typesBack();
	}

}