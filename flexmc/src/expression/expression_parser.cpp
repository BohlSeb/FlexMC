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
            return (tok.type == Token::Type::op && tok.context.maybe_prefix)
                   || (tok.type == Token::Type::lparen)
                   || (tok.type == Token::Type::lbracket);
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
                    auto msg = fmt::format("Unmatched parenthesis or bracket: \"(\" or \"[\", got \"{}\" ({})",
                                           tok.value,
                                           tok.type2String());
                    report.setError(msg, tok.start, tok.size);
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
                error_msg << fmt::format("Expected parenthesis or bracket, got \"{}\" ({})",
                                         tok.value,
                                         tok.type2String());
            }
            else if (operators.back().context.num_args > 0)
            {
                error_msg << "Unexpected comma encountered within parentheses or brackets";
            }
            else if (operators.back().type == Token::Type::lbracket)
            {
                error_msg << "Empty list not allowed: \"[]\"";
            }
            else if (operators.back().type != Token::Type::lparen)
            {
                error_msg << fmt::format("Expected empty argument list  \"()\", got \"{}\" ({})",
                                         tok.value,
                                         tok.type2String());
            }
            if (!error_msg.str().empty())
            {
                report.setError(error_msg.str(), tok.start, tok.size);
                return State::error;
            }
            return State::have_operand;
        }

        State incrementArgsCount(const Token &tok,
                                 std::vector<Token> &postfix,
                                 std::vector<Token> &operators,
                                 MaybeError &report)
        {
            std::string msg = "Unmatched parenthesis or bracket: \")\" or \"]\"";
            msg += " or badly placed comma \",\"";
            if (operators.empty())
            {
                report.setError(msg, tok.start, tok.size);
                return State::error;
            }
            Token::Type operator_t = operators.back().type;
            while ((operator_t != Token::Type::lparen) && (operator_t != Token::Type::lbracket))
            {
                postfix.push_back(operators.back());
                operators.pop_back();
                if (operators.empty())
                {
                    report.setError(msg, tok.start, tok.size);
                    return State::error;
                }
                operator_t = operators.back().type;
            }
            operators.back().context.num_args += 1;
            return State::want_operand;
        }

        State makeReduceOperator(const Token &tok,
                                 std::vector<Token> &postfix,
                                 std::vector<Token> &operators,
                                 MaybeError &report)
        {
            auto msg = fmt::format("Unmatched parenthesis or bracket: \"{}\"", tok.value);
            if (operators.empty())
            {
                report.setError(msg, tok.start, tok.size);
                return State::error;
            }
            Token::Type operator_t = operators.back().type;
            Token::Type stop = (tok.type == Token::Type::rparen) ? Token::Type::lparen : Token::Type::lbracket;
            while (operator_t != stop)
            {
                postfix.push_back(operators.back());
                operators.pop_back();
                if (operators.empty())
                {
                    report.setError(msg, tok.start, tok.size);
                    return State::error;
                }
                operator_t = operators.back().type;
            }
            Token left_close = operators.back();
            size_t num_args = left_close.context.num_args + 1;
            if (left_close.type == Token::Type::lparen && left_close.context.is_infix)
            {
                postfix.push_back(Tokens::makeCall(num_args, tok.start));
            }
            else if (left_close.type == Token::Type::lbracket && left_close.context.is_prefix)
            {
                postfix.push_back(Tokens::makeAppend(num_args, tok.start));
            }
            else if (left_close.type == Token::Type::lbracket && left_close.context.is_infix)
            {
                postfix.push_back(Tokens::makeIndex(num_args, tok.start));
            }
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

        State checkParenthesisAfterFunction(const Token &function, std::deque<Token> &infix, MaybeError &report)
        {
            auto found = false;
            for (const auto &token: infix)
            {
                if (!isSpace(token.type)) {
                    if (token.type == Token::Type::lparen) {
                        found = true;
                    }
                    break;
                }
            }
            if (!found) {
                report.setError("Expected opening parenthesis \"(\" after function", function.start, function.size);
                return State::error;
            }
            return State::have_operand;
        }

        State wantOperand(std::deque<Token> &infix,
                          std::vector<Token> &postfix,
                          std::vector<Token> &operators,
                          MaybeError &report)
        {
            using type = Token::Type;

            assert(!infix.empty());

            Token next = infix.front();
            Token::Type t = next.type;

            if (t == Token::Type::undefined)
            {
                report.setError("Does not start with a valid language token", next.start, next.size);
                return State::error;
            }
            if (t == Token::Type::eof)
            {
                report.setError(
                        "Expected a variable, value, function name or a prefix operator, got end of line",
                        next.start,
                        0
                );
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
                if (t == Token::Type::fun) {
                    return checkParenthesisAfterFunction(postfix.back(), infix, report);
                }
                return State::have_operand;
            }
            if (isPrefixOp(next))
            {
                assert(!next.context.is_infix);
                next.context.is_prefix = true;
                next.context.is_infix = false;
                // jump mul/div
                if ((next.value == PLUS) || (next.value == MINUS))
                {
                    next.context.precedence += 2;
                }
                operators.push_back(next);
                infix.pop_front();
                return State::want_operand;
            }

            // Unmatched parenthesis or "()" expected (function with 0 args)
            if ((t == type::rparen) || (t == type::rbracket))
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
                    "Expected a variable, value, function name or a prefix operator, got \"{0}\" ({1})",
                    next.value,
                    next.type2String()
            );
            report.setError(msg, next.start, next.size);
            return State::error;
        }

        State haveOperand(std::deque<Token> &infix,
                          std::vector<Token> &postfix,
                          std::vector<Token> &operators,
                          MaybeError &report)
        {
            using type = Token::Type;

            assert(!infix.empty());

            Token next = infix.front();
            Token::Type t = next.type;

            if (t == Token::Type::undefined)
            {
                report.setError("Does not start with a valid language token", next.start, next.size);
                return State::error;
            }
            if (isSpace(t))
            {
                infix.pop_front();
                return State::have_operand;
            }
            if (t == type::eof)
            {
                return terminate(next, postfix, operators, report);
            }
            // wiki page shunting yard
            if ((t == type::lparen) || (t == type::lbracket))
            {
                assert(!next.context.is_prefix);
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
            if ((next.type == type::rparen) || (next.type == type::rbracket))
            {
                State s = makeReduceOperator(next, postfix, operators, report);
                if (s == State::have_operand)
                {
                    infix.pop_front();
                }
                return s;
            }
            if ((next.type == type::op) && (next.context.maybe_infix))
            {
                State s = pushOperators(next, postfix, operators);
                infix.pop_front();
                return s;
            }
            std::string msg = fmt::format(
                    "Expected an operator, got \"{0}\" of type {1}",
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
