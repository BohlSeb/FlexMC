#pragma once

#include <deque>

#include "language_error.h"


namespace flexMC
{

    struct LineParseResult
    {
        const std::deque<Token> statement_begin;
        const std::deque<Token> expression_infix;

        LineParseResult() : statement_begin({}), expression_infix({})
        {}

        LineParseResult(const std::deque<Token> &line_start,
                        const std::deque<Token> &expr_infix)
            : statement_begin(line_start), expression_infix(expr_infix)
        {}
    };

    namespace statementPUtils {

        std::pair<MaybeError, std::deque<Token>> stripStartOfLine(std::deque<Token> &line);

        std::deque<Token> makePaymentExpression(MaybeError &report,
                                                std::deque<Token> &line_start,
                                                const std::deque<Token> &rest_of_line);

    }

    std::pair<MaybeError, LineParseResult> parseLine(std::deque<Token> line_infix);

}