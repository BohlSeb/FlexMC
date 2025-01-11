#include <cassert>
#include <sstream>
#include <fmt/format.h>

#include "expression_parser.h"

// We try to implement this exceptional answer to a question about the shunting yard algorithm on stackoverflow:
// https://stackoverflow.com/questions/16380234/handling-extra-operators-in-shunting-yard/16392115#16392115
// https://en.wikipedia.org/wiki/Shunting_yard_algorithm


namespace flexMC
{
    namespace
    {

        enum class State
        {
            want_operand,
            have_operand,
            end,
            error
        };

        bool isSpace(const Token::Type &t)
        { return ((t == Token::Type::wsp) || (t == Token::Type::tab)); }

        bool isOperand(const Token::Type &t)
        { return (t == Token::Type::num || t == Token::Type::id || t == Token::Type::fun); }

        bool isPrefixOp(const Token &tok)
        {
            using
            enum Token::Type;
            return (tok.type == op && tok.context.maybe_prefix) || (tok.type == lparen);
        }

        State terminate(const Token &tok,
                        std::vector<Token> &postfix,
                        std::vector<Token> &operators,
                        MaybeError &report)
        {
            while (!operators.empty())
            {
                Token op = operators.back();
                if ((op.type == Token::Type::lparen) || (op.type == Token::Type::lbracket))
                {
                    auto msg = fmt::format(R"(Unmatched parenthesis "(" or bracket "[", got "{}" ({}))",
                                           tok.value,
                                           tok.type2String());
                    report.setError(msg, tok);
                    return State::error;
                }
                postfix.push_back(op);
                operators.pop_back();
            }
            return State::end;
        }

        State noArgsOrError(const Token &tok, const std::vector<Token> &operators, MaybeError &report)
        {
            std::stringstream error_msg("");
            if (operators.empty())
            {
                error_msg << R"_(Expected empty function argument list "()" )_";
            }
            else if (operators.back().type != Token::Type::lparen)
            {
                error_msg << fmt::format(R"_(Expected left parenthesis "()", got "{})" ({}) )_",
                                         operators.back().value,
                                         operators.back().type2String());
            }
            else if (!operators.back().context.is_infix)
            {
                error_msg << R"_(Empty parentheses "()" not allowed other than for an empty function argument list)_";
            }
            else if (operators.back().context.num_args > 0)
            {
                error_msg << R"_(Unexpected comma encountered within empty parentheses "()")_";
            }
            if (!error_msg.str().empty())
            {
                report.setError(error_msg.str(), tok);
                return State::error;
            }
            return State::have_operand;
        }

        State incrementArgsCount(const Token &tok,
                                 std::vector<Token> &postfix,
                                 std::vector<Token> &operators,
                                 MaybeError &report)
        {
            std::string msg = R"_(Unmatched parenthesis or bracket: ")" or "]")_";
            msg += R"( or badly placed comma ",")";
            if (operators.empty())
            {
                report.setError(msg, tok);
                return State::error;
            }
            Token::Type operator_t = operators.back().type;
            while ((operator_t != Token::Type::lparen) && (operator_t != Token::Type::lbracket))
            {
                postfix.push_back(operators.back());
                operators.pop_back();
                if (operators.empty())
                {
                    report.setError(msg, tok);
                    return State::error;
                }
                operator_t = operators.back().type;
            }
            operators.back().context.num_args += 1;
            return State::want_operand;
        }

        State consumeParentheses(const Token &tok,
                                 std::vector<Token> &postfix,
                                 std::vector<Token> &operators,
                                 MaybeError &report)
        {
            auto msg = R"_(Unmatched parenthesis ")")_";
            if (operators.empty())
            {
                report.setError(msg, tok);
                return State::error;
            }
            Token::Type operator_t = operators.back().type;
            while (operator_t != Token::Type::lparen)
            {
                if (operator_t == Token::Type::lbracket)
                {
                    report.setError(R"_(While parsing parenthesis ")": Unexpected bracket encountered)_",
                                    operators.back());
                    return State::error;
                }
                postfix.push_back(operators.back());
                operators.pop_back();
                if (operators.empty())
                {
                    report.setError(msg, tok);
                    return State::error;
                }
                operator_t = operators.back().type;
            }
            Token l_paren = operators.back();
            std::size_t num_args = l_paren.context.num_args + 1;
            if (l_paren.context.is_infix)
            {
                postfix.push_back(Tokens::makeCall(num_args, tok.start));
            }
            else if (num_args >= 2)
            {
                postfix.push_back(Tokens::makeAppend(num_args, tok.start));
            }
            operators.pop_back();
            return State::have_operand;
        }

        State consumeBrackets(const Token &tok,
                              std::vector<Token> &postfix,
                              std::vector<Token> &operators,
                              MaybeError &report)
        {
            auto msg = R"(Unmatched bracket "]")";
            if (operators.empty())
            {
                report.setError(msg, tok);
                return State::error;
            }
            Token::Type operator_t = operators.back().type;
            while (operator_t != Token::Type::lbracket)
            {
                if (operator_t == Token::Type::lparen)
                {
                    report.setError(R"(While parsing bracket "]": Unexpected parenthesis encountered)",
                                    operators.back());
                    return State::error;
                }
                postfix.push_back(operators.back());
                operators.pop_back();
                if (operators.empty())
                {
                    report.setError(msg, tok);
                    return State::error;
                }
                operator_t = operators.back().type;
            }
            if (operators.back().context.num_args > 0)
            {
                report.setError("Multiple arguments not allowed for <Vector> subscripting",
                                operators.back());
                return State::error;

            }
            postfix.push_back(Tokens::makeIndex(tok.start));
            operators.pop_back();
            return State::have_operand;
        }

        State pushOperators(Token tok,
                            std::vector<Token> &postfix,
                            std::vector<Token> &operators)
        {
            while (!operators.empty())
            {
                ParsingContext op_c = operators.back().context;
                ParsingContext input_c = tok.context;
                bool higher = op_c.precedence > input_c.precedence;
                bool barely_higher = (op_c.precedence == input_c.precedence) && input_c.left_associative;
                if (higher || barely_higher)
                {
                    postfix.push_back(operators.back());
                    operators.pop_back();
                }
                else
                {
                    break;
                }
            }
            assert(!tok.context.is_prefix);
            tok.context.is_infix = true;
            operators.push_back(tok);
            return State::want_operand;
        }

        State checkParenthesisAfterFunction(const Token &function, const std::deque<Token> &infix, MaybeError &report)
        {
            auto found = false;
            for (const auto &token: infix)
            {
                if (!isSpace(token.type))
                {
                    if (token.type == Token::Type::lparen)
                    {
                        found = true;
                    }
                    break;
                }
            }
            if (!found)
            {
                report.setError(R"(Expected opening parenthesis "(" after function)", function);
                return State::error;
            }
            return State::have_operand;
        }

        State wantOperand(std::deque<Token> &infix,
                          std::vector<Token> &postfix,
                          std::vector<Token> &operators,
                          MaybeError &report)
        {
            using
            enum Token::Type;

            assert(!infix.empty());

            Token next = infix.front();
            Token::Type t = next.type;

            if (t == undefined)
            {
                report.setError("Does not start with a valid language token", next);
                return State::error;
            }

            if (t == eof)
            {
                report.setError("Expected a variable, value, function name or a prefix operator, got end of line",
                                next);
                return State::error;
            }

            if (isSpace(t))
            {
                infix.pop_front();
                return State::want_operand;
            }

            if (isOperand(t))
            {
                postfix.push_back(next);
                infix.pop_front();
                if (t == fun)
                {
                    return checkParenthesisAfterFunction(postfix.back(), infix, report);
                }
                return State::have_operand;
            }

            if (isPrefixOp(next))
            {
                assert(!next.context.is_infix);
                next.context.is_prefix = true;
                // jump mul/div
                if ((next.value == PLUS) || (next.value == MINUS))
                {
                    next.context.precedence += 2;
                }
                operators.push_back(next);
                infix.pop_front();
                return State::want_operand;
            }

            // "()" expected (function with 0 args)
            if (t == rparen)
            {
                State s = noArgsOrError(next, operators, report);
                if (s != State::error)
                {
                    operators.pop_back();
                    operators.push_back(Tokens::makeCall(0, 0));
                    infix.pop_front();
                }
                return s;
            }

            std::string msg = fmt::format(
                    R"(Expected a variable, value, function name or a prefix operator, got "{}" ({}))",
                    next.value,
                    next.type2String()
            );
            report.setError(msg, next);
            return State::error;
        }

        State haveOperand(std::deque<Token> &infix,
                          std::vector<Token> &postfix,
                          std::vector<Token> &operators,
                          MaybeError &report)
        {
            using
            enum Token::Type;

            assert(!infix.empty());

            Token next = infix.front();
            Token::Type t = next.type;

            if (t == undefined)
            {
                report.setError("Does not start with a valid language token", next);
                return State::error;
            }

            if (isSpace(t))
            {
                infix.pop_front();
                return State::have_operand;
            }

            if (t == eof)
            {
                return terminate(next, postfix, operators, report);
            }

            // wiki page shunting yard
            if ((t == lparen) || (t == lbracket))
            {
                assert(!next.context.is_prefix);
                assert(next.context.maybe_infix);
                next.context.is_infix = true;
                operators.push_back(next);
                infix.pop_front();
                return State::want_operand;
            }

            if (next.value == COMMA)
            {
                State s = incrementArgsCount(next, postfix, operators, report);
                if (s == State::want_operand)
                {
                    infix.pop_front();
                }
                return s;
            }

            if (next.type == rparen)
            {
                State s = consumeParentheses(next, postfix, operators, report);
                if (s == State::have_operand)
                {
                    infix.pop_front();
                }
                return s;
            }

            if (next.type == rbracket)
            {
                State s = consumeBrackets(next, postfix, operators, report);
                if (s == State::have_operand)
                {
                    infix.pop_front();
                }
                return s;
            }

            if ((next.type == op) && (next.context.maybe_infix))
            {
                State s = pushOperators(next, postfix, operators);
                infix.pop_front();
                return s;
            }

            std::string msg = fmt::format(
                    R"(Expected an operator, got "{}" of type {})",
                    next.value,
                    next.type2String()
            );

            report.setError(msg, next.start, next.size);
            return State::error;
        }
    }

    std::pair<MaybeError, std::vector<Token>> infixToPostfix(const std::deque<Token> &infix)
    {
        assert(!infix.empty());
        std::deque<Token> infix_(infix);
        std::vector<Token> out;
        std::vector<Token> operators;
        Token first = infix_.front();

        MaybeError report;

        State current = wantOperand(infix_, out, operators, report);
        while ((current != State::end) && (current != State::error))
        {
            if (current == State::want_operand)
            {
                current = wantOperand(infix_, out, operators, report);
            }
            else
            {
                current = haveOperand(infix_, out, operators, report);
            }
        }
        return std::make_pair(report, out);
    }

}
