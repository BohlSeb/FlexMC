#include "tokens.h"


namespace flexMC
{

    // this is stupid but apparently not straight forward
    // https://stackoverflow.com/questions/3342726/c-print-out-enum-value-as-text
    std::string Token::type2String() const
    {
        switch (type)
        {
            using
            enum flexMC::Token::Type;
            case eof:
                return "end of line";
            case wsp:
                return "space";
            case tab:
                return "tab";
            case fun:
                return "function";
            case op:
                return "operator";
            case num:
                return "number";
            case keyW:
                return "keyword";
            case id:
                return "variable name";
            case lparen:
                return "opening parenthesis";
            case rparen:
                return "closing parenthesis";
            case lbracket:
                return "opening bracket";
            case rbracket:
                return "closing bracket";
            case undefined:
                return "undefined or not recognized";
            case call_:
                return "call_";
            case append_:
                return "append_";
            case index_:
                return "index_";
            default:
                return "undefined or not recognized";
        }
    }

    std::string Token::toString() const
    {
        using
        enum flexMC::Token::Type;
        std::string out;
        switch (type)
        {
            case wsp:
            case tab:
            case eof:
                out = type2String();
                break;
            default:
                out = value;
        }
        return "Tok(t=" + type2String() + ", v=" + out + ")";
    }

    Token::Type Tokens::getType(const std::string &symbol)
    {
        auto lookUp = Tokens::TYPES.find(symbol);
        if (lookUp == Tokens::TYPES.end())
        {
            return Token::Type::undefined;
        }
        else
        {
            return lookUp->second;
        }
    }

    // currently using python operator precedence
    // https://docs.python.org/3/reference/expressions.html

    Token Tokens::makeCall(const size_t &num_args, const size_t &at)
    {
        auto call = Token(Token::Type::call_, CALL_, at);
        call.context.num_args = num_args;
        call.context.precedence = 10;
        return call;
    }

    Token Tokens::makeAppend(const size_t &num_args, const size_t &at)
    {
        auto call = Token(Token::Type::append_, APPEND_, at);
        call.context.num_args = num_args;
        call.context.precedence = 10;
        return call;
    }

    Token Tokens::makeIndex(const size_t &at)
    {
        auto call = Token(Token::Type::index_, INDEX_, at);
        call.context.num_args = 1;
        call.context.precedence = 10;
        return call;
    }

    Token Tokens::makeOperator(const Token::Type t, const std::string &val, const size_t &at)
    {
        ParsingContext context;
        if (val == POW)
        {
            context.precedence = 9;
            context.left_associative = false;
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        // leave precedence gap to above for unary +/-
        if ((val == MUL) || (val == DIV))
        {
            context.precedence = 7;
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        if ((val == PLUS) || (val == MINUS))
        {
            context.precedence = 6;
            context.maybe_infix = true;
            context.maybe_prefix = true;
            return {t, val, at, context};
        }
        if ((val == LT) ||
            (val == GT) ||
            (val == LE) ||
            (val == GE) ||
            (val == SMOOTH_LT) ||
            (val == SMOOTH_GT))
        {
            context.precedence = 5;
            context.left_associative = false;
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        if (val == NOT)
        {
            context.precedence = 4;
            context.left_associative = false;
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        if (val == AND)
        {
            context.precedence = 3;
            context.left_associative = false;
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        if (val == OR)
        {
            context.precedence = 2;
            context.left_associative = false;
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        if (val == COMMA)
        {
            context.maybe_infix = true;
            context.is_infix = true;
            return {t, val, at, context};
        }
        return Token(Token::Type::undefined, val, at);
    }

    Token Tokens::makeContextualized(const std::string &val, const size_t &at)
    {
        using
        enum Token::Type;
        Token::Type t = getType(val);
        if (t == id || t == keyW || t == fun || t == num)
        {
            return {t, val, at};
        }
        if (t == wsp)
        {
            return {t, " ", at};
        }
        if (t == op)
        {
            return makeOperator(t, val, at);
        }
        if (t == lparen)
        {
            ParsingContext context;
            context.precedence = 0;
            context.maybe_infix = true;
            context.maybe_prefix = true;
            return {t, val, at, context};
        }
        if (t == lbracket)
        {
            ParsingContext context;
            context.precedence = 0;
            context.maybe_infix = true;
            return {t, val, at, context};
        }
        if (t == tab)
        {
            return {t, "    ", at};
        }
        if (t == eof)
        {
            return {t, "", at};
        }
        if ((t == rparen) || (t == rbracket))
        {
            return {t, val, at};
        }
        return {undefined, val, at};
    }
}