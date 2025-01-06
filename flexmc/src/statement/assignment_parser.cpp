#include <cassert>
#include <sstream>
#include <fmt/format.h>

#include "statement_definitions.h"
#include "assignment_parser.h"


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

        using iteratorType = std::vector<assignment::Option>::const_iterator;

        iteratorType lookUp(const iteratorType &begin, const iteratorType &end, const Token &token)
        {
            return std::ranges::find_if(
                begin, end,
                [token](const assignment::Option &op)
                { return op.check_type ? op.type == token.type : op.value == token.value; }
            );
        }

        std::string printOptions(iteratorType begin, const iteratorType &end, const std::string &context)
        {
            std::vector<std::string> options_out;
            while (begin != end)
            {
                if (begin->check_type)
                {
                    options_out.push_back(fmt::format("<{}>", Tokens::printType(begin->type)));
                }
                else
                {
                    options_out.push_back(fmt::format(R"_("{}")_", begin->value));
                }
                ++begin;
            }
            std::stringstream msg;
            msg << context << ", " << R"_(admissible <type> or "value" options are: [)_";
            msg << fmt::to_string(fmt::join(options_out, ", ")) << "]";
            return msg.str();
        }

        void setError(MaybeError &report,
                      const int &err_code,
                      const Token &token,
                      const iteratorType &options_begin,
                      const iteratorType &options_end)
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
    }

    std::pair<MaybeError, std::deque<Token>> stripStartOfLine(std::deque<Token> line)
    {
        assert(!line.empty());
        MaybeError report;
        auto c_beg = assignment::OPTIONS.begin();
        auto c_end = assignment::OPTIONS.end();

        const std::size_t spaces = countFrontSpaces(line);
        const int err_code = stripFront(line);

        if (err_code > 0)  // 1 or 2
        {
            setError(report, err_code, line.front(), c_beg, c_end);
            return {report, {}};
        }
        if (spaces != 0 && spaces != 4)
        {
            auto tok = Token(Token::Type::id, std::string(spaces, ' '), 0);
            setError(report, 3, tok, c_beg, c_end);
            return {report, {}};
        }

        std::deque<Token> line_first_part;

        if (spaces == 4)
        {
            auto tok = Token(Token::Type::tab, "    ", 0);
            auto tab_look_up = lookUp(c_beg, c_end, tok);
            if (tab_look_up == c_end)
            {
                setError(report, 4, tok, c_beg, c_end);
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
                setError(report, e_code, line.front(), c_beg, c_end);
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
                setError(report, 5, next, c_beg, c_end);
            }
        }

        return {report, line_first_part};
    }
}