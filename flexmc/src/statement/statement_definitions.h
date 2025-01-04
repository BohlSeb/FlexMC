#pragma once

#include <string>
#include <vector>

#include "terminals.h"
#include "tokens.h"

namespace flexMC
{

    namespace assignment
    {

        using
        enum Token::Type;

        struct Option
        {

            const std::string token_value;
            const Token::Type token_type;
            const bool check_with_type;

            const std::vector<Option> options;

            Option(const std::string &token_value_,
                   const Token::Type token_type_,
                   bool check_with_type_,
                   const std::vector<Option> options_) :
                token_value(token_value_),
                token_type(token_type_),
                check_with_type(check_with_type_),
                options(options_)
            {};

            Option(const std::string &token_value_,
                   const Token::Type token_type_,
                   bool check_type_only_) :
                token_value(token_value_),
                token_type(token_type_),
                check_with_type(check_type_only_)
            {};


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
                    Option(ASSIGN, undefined, false),
                }),
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
                    Option(ASSIGN, undefined, false),
                }),
                Option(TERMINATE, undefined, false),
            }),
            Option(ELSE, undefined, false),
        };
    }
}