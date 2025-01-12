#pragma once

#include <string>
#include <vector>

#include "terminals.h"
#include "tokens.h"


namespace flexMC::statement {

    using
    enum Token::Type;

    struct Option {

        const std::string value;
        const Token::Type type;
        const bool check_type;

        const std::vector<Option> options;

        Option(std::string token_value, Token::Type token_type, bool check_with_type,
               const std::vector<Option> &next_options) :
            value(std::move(token_value)),
            type(token_type),
            check_type(check_with_type),
            options(next_options) {};

        Option(std::string token_value, Token::Type token_type, bool check_with_type) :
            value(std::move(token_value)),
            type(token_type),
            check_type(check_with_type) {};

    };

    const std::vector<Option> OPTIONS = {
        Option("", id, true, {
            Option("", id, true, {
                Option(ASSIGN, undefined, false),
                Option(PLUS_ASSIGN, undefined, false),
                Option(MINUS_ASSIGN, undefined, false),
                Option(MUL_ASSIGN, undefined, false),
                Option(DIV_ASSIGN, undefined, false),
                Option(POW_ASSIGN, undefined, false),
            }),
            Option(IF, undefined, false),
            Option(PAY, undefined, false, {
                Option(L_PAREN, undefined, false),
            }),
            Option(PAY_AT, undefined, false, {
                Option(L_PAREN, undefined, false),
            })
        }),
        Option(CONTINUOUS, undefined, false, {
            Option("", id, true, {
                Option(ASSIGN, undefined, false),
                Option(PLUS_ASSIGN, undefined, false),
                Option(MINUS_ASSIGN, undefined, false),
                Option(MUL_ASSIGN, undefined, false),
                Option(DIV_ASSIGN, undefined, false),
                Option(POW_ASSIGN, undefined, false),
            }),
            Option(IF, undefined, false),
        }),
        Option("", tab, true, {
            Option("", id, true, {
                Option(ASSIGN, undefined, false),
                Option(PLUS_ASSIGN, undefined, false),
                Option(MINUS_ASSIGN, undefined, false),
                Option(MUL_ASSIGN, undefined, false),
                Option(DIV_ASSIGN, undefined, false),
                Option(POW_ASSIGN, undefined, false),
            }),
            Option(PAY, undefined, false, {
                Option(L_PAREN, undefined, false),
            }),
            Option(PAY_AT, undefined, false, {
                Option(L_PAREN, undefined, false),
            }),
            Option(TERMINATE, undefined, false),
        }),
        Option(ELSE, undefined, false),
    };
}
