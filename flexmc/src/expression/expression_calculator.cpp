
#include "lexer.h"
#include "expression_calculator.h"


namespace flexMC
{

    std::vector<Token> StaticExpressionCalculator::initialize(const std::string &infix)
    {
        if (infix.empty())
        {
            throw std::runtime_error("Empty string not allowed");
        }
        Lexer lexer;
        const std::deque<Token> inf_ = lexer.tokenize(infix);
        const auto [p_report, postfix] = infixToPostfix(inf_);
        if (p_report.isError())
        {
            std::string msg = printError("Syntax / Parsing", infix, p_report);
            throw std::runtime_error(msg);
        }
        return postfix;
    }


    void StaticExpressionCalculator::compile()
    {
        expression_.clear();
        const auto [e_report, c_report] = compileExpression(postfix_, expression_, storage_);
        if (e_report.isError())
        {
            std::string msg = printError("Syntax / Parsing", infix_, e_report);
            throw std::runtime_error(msg);
        }
        // should reserve memory for stacks here from c_report
        return_type_ = c_report.ret_type;
        is_compiled_ = true;
    }

    SCALAR StaticExpressionCalculator::calculateScalar()
    {
        checkAndCalculate(CType::scalar);
        SCALAR result = stacks_.scalars().back();
        stacks_.scalars().pop_back();
        if (!stacks_.ready()) {
            throw std::runtime_error("Internal error, stacks not ready after popping result");
        }
        return result;
    }

    VECTOR StaticExpressionCalculator::calculateVector()
    {
        checkAndCalculate(CType::vector);
        VECTOR result = stacks_.vectorResult();
        stacks_.popVectorResult();
        if (!stacks_.ready()) {
            throw std::runtime_error("Internal error, stacks not ready after popping result");
        }
        return result;
    }

    void StaticExpressionCalculator::checkAndCalculate(const CType &request_return_type)
    {
        if (!is_compiled_)
        {
            compile();
        }
        if (request_return_type != return_type_)
        {
            std::stringstream msg{"Expression "};
            msg << "\"" << infix_ << "\"" << "evaluates to ";
            msg << cType2Str(return_type_) << ", not ";
            msg << cType2Str(request_return_type);
            throw std::runtime_error(msg.str());
        }
        if (!stacks_.ready())
        {
            throw std::runtime_error("Internal error: Calculation stacks not ready");
        }
        // try catch overflow
        expression_(stacks_);
    }


}
