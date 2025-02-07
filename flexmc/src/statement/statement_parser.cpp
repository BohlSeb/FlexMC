#include <cassert>
#include <sstream>
#include <fmt/format.h>
#include <ranges>
#include <algorithm>
#include <numeric>

#include "statement_definitions.h"
#include "expression_parser.h"
#include "statement_parser.h"


namespace flexMC {
    namespace {
        using
        enum Token::Type;

        auto IS_NOT_SPACE = [](const Token &t) { return ((t.type != wsp) && (t.type != tab)); };

        auto UNDEFINED = [](const Token &t) { return t.type == undefined; };

        std::size_t countFrontSpaces(const std::deque<Token> &line) {
            const auto end = std::ranges::find_if(line, IS_NOT_SPACE);
            return std::accumulate(line.begin(), end, 0, [](const std::size_t &acc, const Token &t) {
                return acc + (t.type == wsp ? 1 : 4);
            });
        }

        auto findStatementOption(const auto &begin, const auto &end, const Token &token) {
            return std::ranges::find_if(
                begin, end,
                [token](const statement::Option &op) {
                    return op.check_type ? op.type == token.type : op.value == token.value;
                }
            );
        }

        std::string printOptions(auto begin, const auto &end, const std::string &context) {
            std::vector<std::string> options_out;
            for (const auto &option: std::ranges::subrange(begin, end)) {
                options_out.emplace_back(option.check_type
                                             ? fmt::format("<{}>", Tokens::printType(option.type))
                                             : fmt::format(R"_("{}")_", option.value));
            }
            return format("{}, admissible <type> or \"value\" options are: [{}]",
                          context,
                          fmt::join(options_out, ", "));
        }

        void setOptionContextError(MaybeError &report,
                                   const int &err_code,
                                   const Token &token,
                                   const auto &options_begin,
                                   const auto &options_end) {
            assert(!report.isError());
            assert((1 <= err_code) && (err_code <= 5));
            std::stringstream context;
            switch (err_code) {
                case 1:
                    if (token.start == 0) {
                        context << "Empty line encountered";
                    }
                    else {
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

        bool testLineType(const std::deque<Token> &line, const std::string &value) {
            return std::ranges::find_if(line, [value](const auto &t) { return t.value == value; }) != line.end();
        }

        LineType lineType(const std::deque<Token> &start_of_line) {
            // Review options to make stuff like this more concise
            if (testLineType(start_of_line, PAY)) {
                return LineType::pay;
            }
            if (testLineType(start_of_line, PAY_AT)) {
                return LineType::pay_at;
            }
            if (testLineType(start_of_line, IF)) {
                return LineType::if_;
            }
            if (testLineType(start_of_line, ELSE)) {
                return LineType::else_;
            }
            if (testLineType(start_of_line, TERMINATE)) {
                return LineType::terminate;
            }
            if (testLineType(start_of_line, ASSIGN)) {
                return LineType::assign;
            }
            return LineType::eval;
        }
    }


    std::tuple<MaybeError, std::deque<Token>, std::deque<Token> > lineParseUtils::splitLine(
        const std::size_t &spaces, auto line) {
        MaybeError report;
        auto c_beg = statement::OPTIONS.begin();
        auto c_end = statement::OPTIONS.end();

        std::deque<Token> expression_infix(line.begin(), line.end());
        std::deque<Token> statement_begin;

        if (spaces == 4) {
            const auto indent = Token(tab, "    ", 0);
            const auto indent_it = findStatementOption(c_beg, c_end, indent);
            if (indent_it == c_end) {
                setOptionContextError(report, 4, indent, c_beg, c_end);
                return {report, {}, {}};
            }
            statement_begin.push_back(indent);
            c_beg = indent_it->options.begin();
            c_end = indent_it->options.end();
        }

        while ((!report.isError()) && (c_beg != c_end)) {
            const Token &next = expression_infix.front();
            if (next.type == eof) {
                setOptionContextError(report, 1, line.front(), c_beg, c_end);
                break;
            }
            if (const auto option_it = findStatementOption(c_beg, c_end, next); option_it != c_end) {
                c_beg = option_it->options.begin();
                c_end = option_it->options.end();
                statement_begin.push_back(next);
                expression_infix.pop_front();
            }
            else {
                setOptionContextError(report, 5, next, c_beg, c_end);
            }
        }

        return {report, statement_begin, expression_infix};
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

    std::string paymentError(const int &error_code) {
        std::string context;
        switch (error_code) {
            case 1:
                return R"_(Missing assignment operator \":=\" after \"PAY()\" or \"PAY_AT()\" statement)_";
            case 2:
                return R"_(Missing closing parenthesis ")" before ":=" in "PAY or "PAY_AT" statement)_";
            case 3:
                return
                    R"_(Unexpected token between closing parenthesis \")\" and \":=\" in \"PAY\" or \"PAY_AT\" statement)_";
            default:
                return "Internal Error";
        }
    }

    std::deque<Token> lineParseUtils::makePaymentExpression(MaybeError &report,
                                                            std::deque<Token> &line_start,
                                                            const std::deque<Token> &line_rest) {
        // start_of_line is [..., "PAY" or "PAY_AT", "("] at this point
        // The "(" will be removed from start_of_line

        const auto it_assign = std::ranges::find_if(line_rest, [](const Token &t) { return t.value == ASSIGN; });
        if (it_assign == line_rest.end()) {
            report.setError(paymentError(1), 0, 1);
            return {};
        }

        const auto it_r_paren = std::ranges::find_last_if(line_rest.begin(), it_assign,
                                                          [](const Token &t) { return t.value == R_PAREN; }).begin();
        if (it_r_paren == it_assign) {
            report.setError(paymentError(2), it_assign->start, it_assign->size);
            return {};
        }

        if (std::ranges::find_if(it_r_paren + 1, it_assign, IS_NOT_SPACE) != it_assign) {
            report.setError(paymentError(3), it_assign->start, it_assign->size);
            return {};
        }

        if (const auto [expression_report, _] = infixToPostfix(std::deque(it_assign + 1, line_rest.end()));
            expression_report.isError()) {
            report = expression_report;
            return {};
        }

        // OK
        // Have to create a new deque since Token is not copyable (has const qualified members)
        std::deque l_rest_n(line_rest.begin(), it_r_paren);
        l_rest_n.emplace_back(op, COMMA, it_r_paren->start);
        for (auto it = it_assign + 1; it != line_rest.end(); ++it) {
            l_rest_n.push_back(*it);
        }
        assert(l_rest_n.back().type == eof);
        const auto at = l_rest_n.back().start;
        l_rest_n.pop_back();
        l_rest_n.emplace_back(rparen, R_PAREN, at);
        l_rest_n.emplace_back(eof, "", at); // + 1 ?
        l_rest_n.push_front(line_start.back());
        line_start.pop_back(); // Removing the "("
        l_rest_n.push_front(line_start.back()); // Copying "PAY" or "PAY_AT"
        return l_rest_n;
    };


    std::pair<MaybeError, LineParseResult> parseStartOfLine(const std::deque<Token> &line_infix) {
        if (const auto undef = std::ranges::find_if(line_infix, UNDEFINED); undef != line_infix.end()) {
            MaybeError report;
            setOptionContextError(report, 2, *undef, statement::OPTIONS.begin(), statement::OPTIONS.end());
            return {report, {}};
        }
        const std::size_t spaces = countFrontSpaces(line_infix);
        if ((spaces != 0) && (spaces != 4)) {
            MaybeError report;
            auto tok = Token(id, std::string(spaces, ' '), 0);
            setOptionContextError(report, 3, tok, statement::OPTIONS.begin(), statement::OPTIONS.end());
            return {report, {}};
        }
        auto spaces_filtered = line_infix | std::ranges::views::filter(IS_NOT_SPACE);

        auto [split_rep, statement_begin, expression] = lineParseUtils::splitLine(spaces,
            std::move(spaces_filtered));
        if (split_rep.isError()) {
            return {split_rep, {}};
        }
        MaybeError report;
        const LineType t = lineType(statement_begin);
        if ((t == LineType::pay) || (t == LineType::pay_at)) {
            std::deque<Token> pay_expr = lineParseUtils::makePaymentExpression(report, statement_begin, expression);
            if (report.isError()) {
                return {report, {}};
            }
            return {report, LineParseResult(statement_begin, pay_expr, t)};
        }
        return {report, LineParseResult(statement_begin, expression, t)};
    }
}
