#include <cassert>
#include <fmt/format.h>
#include <sstream>
#include <utility>

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

        std::pair<MaybeError, std::deque<Token>>
        error(const std::string &msg, const std::size_t &at, const std::size_t len)
        {
            MaybeError report;
            report.setError(msg, at, len);
            return {report, {}};
        }

        std::pair<int, std::size_t> stripFront(std::deque<Token> &line)
        {
            assert(!line.empty());
            std::size_t spaces = 0;
            bool empty = true;
            bool not_defined = false;
            Token::Type start = line.front().type;

            while (start != eof)
            {
                if (isSpace(start))
                {
                    spaces += line.front().size;
                    line.pop_front();
                    assert(!line.empty());  // type != eof cannot be last
                    start = line.front().type;
                }
                else if (start == undefined)
                {
                    not_defined = true;
                    break;
                }
                else
                {
                    empty = false;
                    break;
                }
            }
            if (not_defined)
            {
                return {2, 0};
            }
            if (empty)
            {
                return {1, 0};
            }
            return {0, spaces};
        }

        std::vector<assignment::Option>::const_iterator findIndent(const std::vector<assignment::Option> &options)
        {
            return std::ranges::find_if(options, [](const auto &op)
            { return op.token_type == tab; });
        }

        std::string printOptions(const std::string &context, const std::vector<assignment::Option> &options)
        {
            std::vector<std::string> options_;
            for (const auto &option: options)
            {
                if (option.check_with_type)
                {
                    options_.push_back(fmt::format("<{}>", Tokens::printType(option.token_type)));
                }
                else
                {
                    options_.push_back(fmt::format(R"_("{}")_", option.token_value));
                }
            }
            std::stringstream msg;
            msg << context << ", " << R"_(admissible <type> or "value" options are: [)_";
            msg << fmt::to_string(fmt::join(options_, ", ")) << "]";
            return msg.str();
        }

        void checkAndAppend(std::deque<Token> &line,
                            std::deque<Token> &parsed,
                            const std::vector<assignment::Option> &options,
                            MaybeError &report,
                            bool is_indented)
        {
            if (report.isError() || options.empty())
            {
                return;
            }
            if (is_indented)
            {
                auto tab_it = findIndent(options);
                if (tab_it == options.end()) {
                    report.setError(printOptions("Unexpected indentation", options), line.front().start, 4);
                    return;
                }
                parsed.emplace_back(Token::Type::tab, "    ", 0);
                checkAndAppend(line, parsed, tab_it->options, report, false);
                return;
            }
            const auto [err_code, _] = stripFront(line);
            if (err_code == 1)
            {
                report.setError(printOptions("Unexpected end of line", options), line.front().start, 0);
                return;
            }
            if (err_code == 2)
            {
                report.setError(printOptions("Unexpected token encountered", options), line.front().start, 0);
                return;
            }
            const Token next = line.front();
            for (const auto &option: options)
            {
                const bool check_1 = option.check_with_type && (option.token_type == next.type);
                const bool check_2 = (!option.check_with_type) && (option.token_value == next.value);

                if (check_1 || check_2)
                {
                    parsed.push_back(next);
                    line.pop_front();
                    checkAndAppend(line, parsed, option.options, report, false);
                    return;
                }
            }
            const std::string context = fmt::format(R"_(Unexpected token "{}" encountered)_", next.value);
            report.setError(printOptions(context, options), next.start, next.size);
        }
    }

    std::pair<MaybeError, std::deque<Token>> parseLineStart(const std::deque<Token> &line)
    {
        assert(!line.empty());
        auto line_(line);
        const auto [err_code, spaces] = stripFront(line_);
        if (err_code == 1)
        {
            return error("Empty line encountered", 0, 0);
        }
        if (err_code == 2)
        {
            return error("No valid language token found at the start of the line", 0, line.front().size);
        }
        if (spaces != 0 && spaces != 4)
        {
            return error("Bad spaces/tabs encountered, expected 0 or 4 spaces", 0, spaces);
        }
        const bool is_indented = (spaces == 4);
        MaybeError report;
        std::deque<Token> parsed_;
        checkAndAppend(line_, parsed_, assignment::OPTIONS, report, is_indented);
        return {report, parsed_};
    }
}