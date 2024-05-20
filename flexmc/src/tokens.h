#pragma once

#include <unordered_map>

#include "terminals.h"


namespace flexMC
{

    struct ParsingContext
    {

        ParsingContext() : precedence(1)
        {}

        int precedence;
        bool maybe_prefix = false;
        bool maybe_infix = false;
        bool is_prefix = false;
        bool is_infix = false;
        int num_args = 0;
        bool left_associative = true;

    };

    struct Token
    {

        enum class Type
        {
            eof,
            wsp,
            tab,
            op,
            fun,
            num,
            keyW,
            id,
            lparen,
            rparen,
            lbracket,
            rbracket,
            undefined,
            call_,
            append_,
            index_
        };

        explicit Token(Type t, std::string val = "") :
                type(t),
                value(std::move(val)),
                context(ParsingContext())
        {}

        Token(Type t, std::string val, ParsingContext con) :
                type(t),
                value(std::move(val)),
                context(con)
        {}

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string type2String() const;

        const Type type;
        const std::string value;
        ParsingContext context;
    };

    namespace Tokens
    {

        Token::Type getType(const std::string &symbol);

        Token makeContextualized(const std::string &symbol);

        Token makeCall(const int &num_args);

        Token makeAppend(const int &num_args);

        Token makeIndex(const int &num_args);

        const std::unordered_map<std::string, Token::Type> TYPES = {

                {IF,           Token::Type::keyW},
                {THEN,         Token::Type::keyW},
                {ELSE,         Token::Type::keyW},
                {PAY,          Token::Type::keyW},
                {STOP,         Token::Type::keyW},
                {ASSIGN,       Token::Type::keyW},
                {PLUS_ASSIGN,  Token::Type::keyW},
                {MINUS_ASSIGN, Token::Type::keyW},
                {MUL_ASSIGN,   Token::Type::keyW},
                {DIV_ASSIGN,   Token::Type::keyW},
                {POW_ASSIGN,   Token::Type::keyW},

                {EXP,          Token::Type::fun},
                {LOG,          Token::Type::fun},
                {ABS,          Token::Type::fun},
                {SQRT,         Token::Type::fun},
                {SQUARE,       Token::Type::fun},
                {MIN,          Token::Type::fun},
                {MAX,          Token::Type::fun},
                {SUM,          Token::Type::fun},
                {PROD,         Token::Type::fun},
                {ARGMIN,       Token::Type::fun},
                {ARGMAX,       Token::Type::fun},
                {LEN,          Token::Type::fun},

                {PLUS,         Token::Type::op},
                {MINUS,        Token::Type::op},
                {MUL,          Token::Type::op},
                {DIV,          Token::Type::op},
                {POW,          Token::Type::op},
                {LT,           Token::Type::op},
                {GT,           Token::Type::op},
                {LE,           Token::Type::op},
                {GE,           Token::Type::op},
                {NOT,          Token::Type::op},
                {AND,          Token::Type::op},
                {OR,           Token::Type::op},
                {SMOOTH_LT,    Token::Type::op},
                {SMOOTH_GT,    Token::Type::op},
                {COMMA,        Token::Type::op},

                {P_LEFT,       Token::Type::lparen},
                {P_RIGHT,      Token::Type::rparen},
                {B_LEFT,       Token::Type::lbracket},
                {B_RIGHT,      Token::Type::rbracket},

                {"\t",         Token::Type::tab},
                {" ",          Token::Type::wsp},
        };
    }
}
