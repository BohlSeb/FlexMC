#pragma once

#include <deque>

#include "language_error.h"


namespace flexMC {

    struct LineParseResult {
        const std::deque<Token> statement_begin;
        const std::deque<Token> expression_infix;

        LineParseResult() : statement_begin({}), expression_infix({}) {}

        LineParseResult(const std::deque<Token> &line_start,
                        const std::deque<Token> &expr_infix)
            : statement_begin(line_start), expression_infix(expr_infix) {}
    };

    namespace lineParseUtils {

        // Can use auto in function declaration in C++20 according to the accepted answer in the link below.
        // It's an abbreviation for function template.
        // https://stackoverflow.com/questions/29944985/is-there-a-way-to-pass-auto-as-an-argument-in-c
        // type is meant to be the return type of >> std::deque<Token> | std::ranges::views::filter <<.
        std::tuple<MaybeError, std::deque<Token>, std::deque<Token>> splitLine(const std::size_t &spaces,
                                                                               auto line);

        std::deque<Token> makePaymentExpression(MaybeError &report,
                                                std::deque<Token> &line_start,
                                                const std::deque<Token> &rest_of_line);

    }

    std::pair<MaybeError, LineParseResult> parseStartOfLine(const std::deque<Token> &line_infix);

}