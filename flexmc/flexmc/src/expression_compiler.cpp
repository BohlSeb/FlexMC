#include <sstream>
#include "expression_compiler.h"
#include "operation_compiler.h"
#include "operand.h"

namespace flexMC {

	void Expression::evaluate(CalcStacks& stacks) {
		for (auto it = items_.cbegin(); it != items_.cend(); ++it) {
			(*it)->evaluate(stacks);
		}
	}

	const Operands::CompileReport ExpressionCompiler::compile(const std::deque<Token>& post_fix, Expression& expression) {
		Operands operands;
		// Reading backwards
		for (auto tok = post_fix.crbegin(); tok != post_fix.crend(); ++tok) {
			const Token::Type t = tok->type;
			if (t == Token::Type::num) {
				Number num = Number(tok->value);
				num.compile(operands);
				expression.addItem<Number>(num);
				// result_.push_back(std::make_shared<Number>(std::move(num)));
				continue;
			}
			if (t == Token::Type::fun) {
				operands.pushFunc(tok->value);
				continue;
			}
			if (t == Token::Type::call_) {
				Operation function = functionCompiler::compile(tok->context.num_args, operands);
				// result_.push_back(std::make_shared<Operation>(std::move(function)));
				expression.addItem<Operation>(function);
				continue;
			}
			if (t == Token::Type::append_) {
				Operands::Type elem_t = arrayCompiler::compile(tok->context.num_args, operands);
				if (elem_t == Operands::Type::scalar) {
					Vector v = Vector(tok->context.num_args);
					expression.addItem<Vector>(v);
					// result_.push_back(std::make_shared<Vector>(tok->context.num_args));
				}
				continue;
			}
			if (t == Token::Type::op) {
				if (!((tok->value == flexMC::PLUS) && (tok->context.is_prefix))) {
					Operation op = operatorCompiler::compile(*tok, operands);
					expression.addItem<Operation>(op);
					// result_.push_back(std::make_shared<Operation>(std::move(op)));
				}
			}
		}
		if (!operands.haveCompiled()) {
			throw std::runtime_error("Could not compile expression up to its return type");
		}
		Operands::CompileReport report(operands.typesBack(), 
			operands.maxSize(Operands::Type::scalar), 
			operands.maxSize(Operands::Type::vector));
		return report;
	}

}