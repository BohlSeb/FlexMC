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

    std::pair<MaybeError, CompileReport> ExpressionCompiler::compile(const std::vector<Token> &post_fix,
                                                                     Expression &expression)
    {
        Operands operands;
        MaybeError report;
        for (const auto &tok: post_fix)
        {
            if (report.isError())
            {
                break;
            }
            const Token::Type t = tok.type;
            if (t == Token::Type::num)
            {
                double v = compileNumber(tok, operands, report);
                if (!report.isError())
                {
                    expression.addItem<Number>(Number(v));
                }
            }
            else if (t == Token::Type::fun)
            {
                operands.pushFunc(tok);
            }
            else if (t == Token::Type::call_)
            {
                Operation function = functionCompiler::compile(tok.context.num_args, operands, report);
                if (!report.isError())
                {
                    expression.addItem<Operation>(function);
                }
            }
            else if (t == Token::Type::append_)
            {
                Operands::Type elem_t = compileVector(tok.context.num_args, operands, report);
                if (report.isError())
                {
                    report.setPosition(tok.start, 1);
                    break;
                }
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
                    Operation op = operatorCompiler::compile(tok, operands, report);
                    if (!report.isError())
                    {
                        expression.addItem<Operation>(op);
                    }
                }
            }
        }
        if (report.isError())
        {
            return std::make_pair(report, CompileReport(Operands::Type::undefined, 0, 0));
        }
        if (!operands.haveCompiled())
        {
            report.setError("Could not compile expression up to its return type", 0, 0);
            return std::make_pair(report, CompileReport(Operands::Type::undefined, 0, 0));
        }
        CompileReport c_rep{
                operands.typesBack(),
                operands.maxSize(Operands::Type::scalar),
                operands.maxSize(Operands::Type::vector)
        };
        return std::make_pair(report, c_rep);
    }

}