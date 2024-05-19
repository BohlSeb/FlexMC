#include <stdexcept>
#include <ranges>

#include "operand.h"
#include "operation_compiler.h"
#include "expression_compiler.h"


namespace flexMC
{

    void Expression::evaluate(CalcStacks &stacks) const
    {
        for (const auto &item: items_)
        {
            item->evaluate(stacks);
        }
    }

    CompileReport ExpressionCompiler::compile(const std::deque<Token> &post_fix, Expression &expression)
    {
        Operands operands;
        for (const auto &tok: std::ranges::reverse_view(post_fix))
        {
            const Token::Type t = tok.type;
            if (t == Token::Type::num)
            {
                auto num = Number(tok.value);
                Number::compile(operands);
                expression.addItem<Number>(num);
            }
            else if (t == Token::Type::fun)
            {
                operands.pushFunc(tok.value);
            }
            else if (t == Token::Type::call_)
            {
                Operation function = functionCompiler::compile(tok.context.num_args, operands);
                expression.addItem<Operation>(function);
            }
            else if (t == Token::Type::append_)
            {
                Operands::Type elem_t = arrayCompiler::compile(tok.context.num_args, operands);
                if (elem_t == Operands::Type::scalar)
                {
                    auto v = Vector(tok.context.num_args);
                    expression.addItem<Vector>(v);
                }
            }
            else if (t == Token::Type::op)
            {
                if (!(tok.value == flexMC::PLUS && tok.context.is_prefix))
                {
                    Operation op = operatorCompiler::compile(tok, operands);
                    expression.addItem<Operation>(op);
                }
            }
        }
        if (!operands.haveCompiled())
        {
            throw std::runtime_error("Could not compile expression up to its return type");
        }
        return {
                operands.typesBack(),
                operands.maxSize(Operands::Type::scalar),
                operands.maxSize(Operands::Type::vector)
        };
    }

}