#include <cassert>
#include <sstream>
#include <fmt/format.h>
#include <algorithm>
#include <iostream>

#include "statement_definitions.h"
#include "statement_parser.h"


namespace flexMC
{

    namespace
    {

        using
        enum Token::Type;

        bool isSpace(const Token::Type &t)
        { return ((t == wsp) || (t == tab)); }

        std::size_t countFrontSpaces(const std::deque<Token> &line)
        {

            std::size_t spaces = 0;
            for (const auto &token: line)
            {
                if (token.type == wsp)
                {
                    spaces += 1;
                }
                else if (token.type == tab)
                {
                    spaces += 4;
                }
                else
                {
                    break;
                }
            }
            return spaces;
        }

        // returns error code 1: end of line reached, 2: undefined token, 0: OK
        int stripFront(std::deque<Token> &line)
        {
            assert(!line.empty());
            Token::Type start = line.front().type;

            while (start != eof)
            {
                if (isSpace(start))
                {
                    line.pop_front();
                    assert(!line.empty());  // type != eof cannot be last
                    start = line.front().type;
                }
                else if (start == undefined)
                {
                    return 2;
                }
                else
                {
                    return 0;
                }
            }
            return 1;
        }

        using stdReadOnlyItOpt = std::vector<statement::Option>::const_iterator;

        stdReadOnlyItOpt lookUp(const stdReadOnlyItOpt &begin, const stdReadOnlyItOpt &end, const Token &token)
        {
            return std::ranges::find_if(
                begin, end,
                [token](const statement::Option &op)
                { return op.check_type ? op.type == token.type : op.value == token.value; }
            );
        }

        std::string printOptions(stdReadOnlyItOpt begin, stdReadOnlyItOpt end, const std::string &context)
        {
            std::vector<std::string> options_out;
            while (begin != end)
            {
                if (begin->check_type)
                {
                    options_out.emplace_back(fmt::format("<{}>", Tokens::printType(begin->type)));
                }
                else
                {
                    options_out.emplace_back(fmt::format(R"_("{}")_", begin->value));
                }
                ++begin;
            }
            std::stringstream msg;
            msg << context << ", " << R"_(admissible <type> or "value" options are: [)_";
            msg << fmt::to_string(fmt::join(options_out, ", ")) << "]";
            return msg.str();
        }

        void setOptionContextError(MaybeError &report,
                                   const int &err_code,
                                   const Token &token,
                                   const stdReadOnlyItOpt &options_begin,
                                   const stdReadOnlyItOpt &options_end)
        {
            assert(!report.isError());
            assert((1 <= err_code) && (err_code <= 5));
            std::stringstream context;
            switch (err_code)
            {
                case 1:
                    if (token.start == 0)
                    {
                        context << "Empty line encountered";
                    }
                    else
                    {
                        context << "Unexpected end of line";
                    }
                    break;
                case 2:
                    context << fmt::format("Unrecognizable token \"{}\"", token.value);
                    break;
                case 3:
                    context << "Bad spaces/tabs encountered, expected 0 or 4 spaces";
                    break;
                case 4:
                    context << "Unexpected indentation";
                    break;
                case 5:
                    context << fmt::format("Unexpeceted token \"{}\"", token.value);
                    break;
                default:
                    context << "Internal Error";
                    break;
            }
            report.setError(printOptions(options_begin, options_end, context.str()), token);
        }

        bool isPayLine(const std::deque<Token> &start_of_line)
        {
            if (start_of_line.size() < 2)
            {
                return false;
            }
            auto it_end = start_of_line.rbegin();
            if (it_end->value != L_PAREN)
            {
                return false;
            }
            ++it_end;
            if ((it_end->value != PAY) && (it_end->value != PAY_AT))
            {
                return false;
            }
            return true;
        }


    }


    std::pair<MaybeError, std::deque<Token>> statementPUtils::stripStartOfLine(std::deque<Token> &line)
    {
        assert(!line.empty());
        MaybeError report;
        auto c_beg = statement::OPTIONS.begin();
        auto c_end = statement::OPTIONS.end();

        const std::size_t spaces = countFrontSpaces(line);
        const int err_code = stripFront(line);

        if (err_code > 0)  // 1 or 2
        {
            setOptionContextError(report, err_code, line.front(), c_beg, c_end);
            return {report, {}};
        }
        if (spaces != 0 && spaces != 4)
        {
            auto tok = Token(Token::Type::id, std::string(spaces, ' '), 0);
            setOptionContextError(report, 3, tok, c_beg, c_end);
            return {report, {}};
        }

        std::deque<Token> line_first_part;

        if (spaces == 4)
        {
            auto tok = Token(Token::Type::tab, "    ", 0);
            auto tab_look_up = lookUp(c_beg, c_end, tok);
            if (tab_look_up == c_end)
            {
                setOptionContextError(report, 4, tok, c_beg, c_end);
                return {report, {}};
            }
            line_first_part.push_back(tok);
            c_beg = tab_look_up->options.begin();
            c_end = tab_look_up->options.end();
        }

        while ((!report.isError()) && (c_beg != c_end))
        {
            int e_code = stripFront(line);
            if (e_code > 0)
            {
                setOptionContextError(report, e_code, line.front(), c_beg, c_end);
                break;
            }
            Token next = line.front();
            auto look_up = lookUp(c_beg, c_end, next);
            if (look_up != c_end)
            {
                c_beg = look_up->options.begin();
                c_end = look_up->options.end();
                line_first_part.push_back(next);
                line.pop_front();
                continue;
            }
            else
            {
                setOptionContextError(report, 5, next, c_beg, c_end);
            }
        }

        return {report, line_first_part};
    }


    /*
    * Sketchy functionn to allow for some syntactic sugar in the PAY(...) and PAY_AT(...) statements.
    * We want the statements to look like
    *     PAY(enum_1, enum_2, ..., enum_N) := myDoubleVar
    *     PAY_AT([date expression], enum_1, enum_2, ..., enum_N) := myDoubleVar
    *     PAY_AT([int expression], enum_1, enum_2, ..., enum_N) := myDoubleVar
    * But internally we want to compile as functions of the form
    *    PAY(enum_1, enum_2, ..., enum_N, myDoubleVar)
    *    PAY_AT([date expression], enum_1, enum_2, ..., enum_N, myDoubleVar)
    *    PAY_AT([int expression], enum_1, enum_2, ..., enum_N, myDoubleVar)
    */
    std::deque<Token> statementPUtils::makePaymentExpression(MaybeError &report,
                                                             std::deque<Token> &start_of_line,
                                                             const std::deque<Token> &rest_of_line)
    {
        // start_of_line is [..., "PAY" or "PAY_AT", "("] at this point
        // The "(" will be removed from start_of_line

        // All kind of checks have to be made, otherwise the expression parser will fail with bad error messages

        auto l_r_paren = [](const Token &t)
        { return t.value == R_PAREN; };
        auto l_assign = [](const Token &t)
        { return t.value == ASSIGN; };

        if (std::ranges::find_if(rest_of_line, l_r_paren) == rest_of_line.end())
        {
            report.setError(R"_(Missing closing parenthesis ")" for "PAY" or "PAY_AT" statement)_",
                            0, 1);
            return {};
        }

        auto it_assign = std::ranges::find_if(rest_of_line, l_assign);
        if (it_assign == rest_of_line.end())
        {
            report.setError(R"_(Missing assignment operator ":=" after "PAY()" or "PAY_AT()" statement)_",
                            0, 1);
            return {};
        }

        auto is_not_empty = [](const Token &t)
        { return (t.type != eof && t.type != wsp && t.type != tab && t.type != undefined); };
        if (std::ranges::find_if(it_assign + 1, rest_of_line.end(), is_not_empty) == rest_of_line.end())
        {
            auto at = it_assign->start + it_assign->size;
            report.setError(R"_(No valid token found after ":=" in "PAY" or "PAY_AT" statement)_", at, 0);
            return {};
        }

        auto it_r_paren = std::ranges::find_last_if(rest_of_line.begin(), it_assign, l_r_paren).begin();
        if (it_r_paren == it_assign)
        {
            report.setError(R"_(Missing closing parenthesis ")" before ":=" in "PAY or "PAY_AT" statement)_",
                            it_assign->start, it_assign->size);
            return {};
        }

        if (std::ranges::find_if(it_r_paren + 1, it_assign, is_not_empty) != it_assign)
        {
            report.setError(
                R"_(Unexpected token between closing parenthesis ")" and ":=" in "PAY" or "PAY_AT" statement)_",
                it_assign->start, it_assign->size);
            return {};
        }

        // OK
        // Have to create a new deque since Token is not copyable (has const qualified members)
        std::deque<Token> new_rest_of_line(rest_of_line.begin(), it_r_paren);
        new_rest_of_line.emplace_back(Token::Type::op, COMMA, it_r_paren->start);
        for (auto it = it_assign + 1; it != rest_of_line.end(); ++it)
        {
            new_rest_of_line.push_back(*it);
        }
        assert(new_rest_of_line.back().type == eof);
        auto at = new_rest_of_line.back().start;
        new_rest_of_line.pop_back();
        new_rest_of_line.emplace_back(Token::Type::rparen, R_PAREN, at);
        new_rest_of_line.emplace_back(Token::Type::eof, "", at);  // + 1 ?
        new_rest_of_line.push_front(start_of_line.back());
        start_of_line.pop_back(); // Removing the "("
        new_rest_of_line.push_front(start_of_line.back()); // Copying "PAY" or "PAY_AT"
        return new_rest_of_line;
    };


    std::pair<MaybeError, LineParseResult> parseLine(std::deque<Token> line)
    {
        auto [rep, line_start] = statementPUtils::stripStartOfLine(line);
        if (rep.isError())
        {
            return {rep, {}};
        }
        MaybeError report;
        if (isPayLine(line_start))
        {
            std::deque<Token> new_line = statementPUtils::makePaymentExpression(report, line_start, line);
            if (report.isError())
            {
                return {report, {}};
            }
            return {report, LineParseResult(line_start, new_line)};
        }
        return {report, LineParseResult(line_start, line)};
    }

}