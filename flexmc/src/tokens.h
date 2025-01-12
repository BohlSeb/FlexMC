#pragma once

#include <unordered_map>

#include "terminals.h"
#include "utils.h"


namespace flexMC {

    struct ParsingContext {
        std::size_t precedence = 0;
        bool maybe_prefix = false;
        bool maybe_infix = false;
        bool is_prefix = false;
        bool is_infix = false;
        std::size_t num_args = 0;
        bool left_associative = true;
    };

    struct Token {

        enum class Type {
            eof,
            wsp,
            tab,
            op,
            fun,
            num,
            keyword,
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

        Token(Type t, const std::string &val, const std::size_t &at) :
            type(t),
            value(val),
            start(at),
            size(val.length()) {}

        Token(Type t, const std::string &val, const std::size_t &at, const ParsingContext &con) :
            type(t),
            value(val),
            start(at),
            size(val.length()),
            context(con) {}

        Token(Type t, const std::string &val, const std::size_t &at, const std::size_t &length) :
            type(t),
            value(val),
            start(at),
            size(length) {}

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string type2String() const;

        const Type type;
        const std::string value;
        const std::size_t start;
        const std::size_t size;
        ParsingContext context;
    };


    namespace Tokens {

        Token::Type getType(const std::string &symbol);

        Token makeContextualized(const std::string &val, const std::size_t &at);

        Token makeCall(const std::size_t &num_args, const std::size_t &at);

        Token makeAppend(const std::size_t &num_args, const std::size_t &at);

        Token makeIndex(const std::size_t &at);

        Token makeOperator(const Token::Type &t, const std::string &val, const std::size_t &at);

        std::string printType(const Token::Type &t);

        const std::unordered_map<std::string, Token::Type, SHash, std::equal_to<>> TYPES = {

            {IF,           Token::Type::keyword},
            {CONTINUOUS,   Token::Type::keyword},
            {ELSE,         Token::Type::keyword},
            {TERMINATE,    Token::Type::keyword},
            {ASSIGN,       Token::Type::keyword},
            {PLUS_ASSIGN,  Token::Type::keyword},
            {MINUS_ASSIGN, Token::Type::keyword},
            {MUL_ASSIGN,   Token::Type::keyword},
            {DIV_ASSIGN,   Token::Type::keyword},
            {POW_ASSIGN,   Token::Type::keyword},

            {PAY,          Token::Type::fun},
            {PAY_AT,       Token::Type::fun},

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

            {L_PAREN,      Token::Type::lparen},
            {R_PAREN,      Token::Type::rparen},
            {L_BRACKET,    Token::Type::lbracket},
            {R_BRACKET,    Token::Type::rbracket},

            {"\t",         Token::Type::tab},
            {"    ",       Token::Type::tab},
            {" ",          Token::Type::wsp},
        };
    }
}
