#include "tokens.h"


namespace flexMC
{

    // this is stupid but apparently not straight forward
    // https://stackoverflow.com/questions/3342726/c-print-out-enum-value-as-text
    // TODO: Remove this entirely (tokens do not care about string output)
    std::string Token::type2String() const
    {
        switch (type)
        {
            case Type::eof:
                return "eof";
            case Type::wsp:
                return "wsp";
            case Type::tab:
                return "tab";
            case Type::fun:
                return "fun";
            case Type::op:
                return "op";
            case Type::num:
                return "num";
            case Type::keyW:
                return "keyW";
            case Type::id:
                return "id";
            case Type::lparen:
                return "lparen";
            case Type::rparen:
                return "rparen";
            case Type::lbracket:
                return "lbracket";
            case Type::rbracket:
                return "rbracket";
            case Type::undefined:
                return "undefined";
            case Type::call_:
                return "call_";
            case Type::append_:
                return "append_";
            case Type::index_:
                return "index_";
            default:
                return "undefinedType";
        }
    }

    std::string Token::toString() const
    {
        std::string out;
        if ((type == Type::wsp) ||
            (type == Type::tab) ||
            (type == Type::eof))
        {
            out = type2String();
        }
        else
        {
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

    Token Tokens::makeCall(const int &num_args, const size_t &at)
    {
        Token call = Token(Token::Type::call_, CALL_, at);
        call.context.num_args = num_args;
        call.context.precedence = 10;
        return call;
    }

    Token Tokens::makeAppend(const int &num_args, const size_t &at)
    {
        Token call = Token(Token::Type::append_, APPEND_, at);
        call.context.num_args = num_args;
        call.context.precedence = 10;
        return call;
    }

    Token Tokens::makeIndex(const int &num_args, const size_t &at)
    {
        Token call = Token(Token::Type::index_, INDEX_, at);
        call.context.num_args = num_args;
        call.context.precedence = 10;
        return call;
    }

    // currently using python operator precedence
    // https://docs.python.org/3/reference/expressions.html

    Token Tokens::makeContextualized(const std::string &val, const size_t &at)
    {
        using type = Token::Type;
        type t = getType(val);
        if (t == type::id || t == type::keyW || t == type::fun || t == type::num)
        {
            return {t, val, at};
        }
        if (t == type::wsp)
        {
            return {t, " ", at};
        }
        if ((t == type::op) || (t == type::lparen) || (t == type::lbracket))
        {
            ParsingContext context = ParsingContext();
            if (t == type::op)
            {
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
                return Token(type::undefined, val, at);
            }
            if (t == type::lparen)
            {
                context.precedence = 0;
                context.maybe_infix = true;
                context.maybe_prefix = true;
                return {t, val, at, context};
            }
            // lbracket
            context.precedence = 0;
            context.maybe_prefix = true;
            return {t, val, at, context};
        }
        if (t == type::tab)
        {
            return {t, "    ", at};
        }
        if (t == type::eof)
        {
            return {t, "", at};
        }
        if ((t == type::rparen) || (t == type::rbracket))
        {
            return {t, val, at};
        }
        return {type::undefined, val, at};
    }
}