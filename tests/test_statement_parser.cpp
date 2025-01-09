#include <gtest/gtest.h>

#include <ranges>

#include "lexer.h"
#include "statement_parser.h"

using namespace flexMC;

TEST(StatementParser, StartOfLineStripper)
{

    struct TestCase
    {
        const int n_tokens;
        const std::string infix;
    };

    std::vector<TestCase> valid_cases = {
        {3, "myDate myVar := "},
        {3, "myDate PAY("},
        {3, "myDate PAY ("},
        {3, "myDate PAY_AT("},
        {3, "myDate PAY_AT ("},
        {3, "myDate myVar += "},
        {3, "myDate myVar -= "},
        {3, "myDate myVar *= "},
        {3, "myDate myVar /= "},
        {3, "myDate myVar **= "},
        {3, "CONTINUOUS myVar := "},
        {3, "CONTINUOUS myVar += "},
        {3, "CONTINUOUS myVar -= "},
        {3, "CONTINUOUS myVar *= "},
        {3, "CONTINUOUS myVar /= "},
        {3, "CONTINUOUS myVar **= "},
        {2, "myDate IF "},
        {2, "CONTINUOUS IF "},
        {3, "    myVar := "},
        {3, "    myVar += "},
        {3, "    myVar -= "},
        {3, "    myVar *= "},
        {3, "    myVar /= "},
        {3, "    myVar **= "},
        {3, "    PAY ( "},
        {3, "    PAY( "},
        {3, "    PAY_AT ( "},
        {3, "    PAY_AT( "},
        {2, "    TERMINATE "},
    };

    std::vector<std::string> bad_cases = {
        "myDate myVar myVar := ",
        "myDate := ",
        "PAY myDate PAY := ",
        "PAY_AT myDate PAY := ",
        "myDate CONTINUOUS",
        "BAD_SYMBOL myVar *= ",
        "myVar BAD_SYMBOL *= ",
        "        myVar := ",
        "TERMINATE",
        "IF xxx",
        "",
        "    ",
        "   ",
        "myDate PAY := ",
        "    PAY := ",
        "myDate PAY_AT := ",
        "    PAY_AT := ",
    };

    Lexer lexer;

    for (const auto &c: valid_cases)
    {

        std::deque<Token> tokens = lexer.tokenize(c.infix);
        const auto [parse_report, tokens_front] = statementPUtils::stripStartOfLine(tokens);
        EXPECT_FALSE(parse_report.isError()) << "Expected no error for valid case: " << c.infix;
        EXPECT_EQ(tokens_front.size(), c.n_tokens) << "Token size mismatch for valid case: " << c.infix;

    }

    for (const auto &infix: bad_cases)
    {

        std::deque<Token> tokens = lexer.tokenize(infix);
        const auto [parse_report, tokens_front] = statementPUtils::stripStartOfLine(tokens);
        EXPECT_TRUE(parse_report.isError()) << "Expected error for bad case: " << infix;
        std::cout << printError("Assignment Parser", infix, parse_report) << "\n";

    }

}

TEST(StatementParser, MakePaymentExpressionValid)
{
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    std::deque<Token> rest_of_line = {
        Token(Token::Type::id, "enum_1", 0),
        Token(Token::Type::op, ",", 0),
        Token(Token::Type::id, "enum_2", 0),
        Token(Token::Type::rparen, ")", 0),
        Token(Token::Type::op, ":=", 0),
        Token(Token::Type::id, "myDoubleVar", 0),
        Token(Token::Type::eof, "", 0)
    };

    std::deque<Token> expected = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 0),
        Token(Token::Type::id, "enum_1", 0),
        Token(Token::Type::op, ",", 0),
        Token(Token::Type::id, "enum_2", 0),
        Token(Token::Type::op, ",", 18),
        Token(Token::Type::id, "myDoubleVar", 0),
        Token(Token::Type::rparen, ")", 0),
        Token(Token::Type::eof, "", 0)
    };

    MaybeError report;

    std::deque<Token> result = statementPUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_FALSE(report.isError()) << "Expected no error for valid payment expression";
    ASSERT_EQ(result.size(), expected.size()) << "Token size mismatch for valid payment expression";
    for (const auto [exp, tok]: std::ranges::zip_view(expected, result))
    {
        EXPECT_EQ(tok.type, exp.type) << "Token type mismatch";
        EXPECT_EQ(tok.value, exp.value) << "Token value mismatch";
    }
}

TEST(StatementParser, PaymentMissingClosingParenthesis)
{
    MaybeError report;
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    std::deque<Token> rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::op, ":=", 18),
        Token(Token::Type::id, "myDoubleVar", 21),
        Token(Token::Type::eof, "", 22)
    };

    std::deque<Token> result = statementPUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for missing closing parenthesis";
}

TEST(StatementParser, EmptyRestOfLine)
{
    MaybeError report;
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    std::deque<Token> rest_of_line = {};

    std::deque<Token> result = statementPUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for empty rest of line";
    ASSERT_TRUE(result.empty()) << "Expected empty result for empty rest of line";
}

TEST(StatementParser, NoAssignmentOperator)
{
    MaybeError report;
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    std::deque<Token> rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::rparen, ")", 18),
        Token(Token::Type::eof, "", 19)
    };

    std::deque<Token> result = statementPUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for missing assignment operator";
    ASSERT_TRUE(result.empty()) << "Expected empty result for missing assignment operator";
}

TEST(StatementParser, NoTokensAfterAssignmentOperator)
{
    MaybeError report;
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    std::deque<Token> rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::rparen, ")", 18),
        Token(Token::Type::op, ":=", 20),
        Token(Token::Type::eof, "", 22)
    };

    std::deque<Token> result = statementPUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for no tokens after assignment operator";
    ASSERT_TRUE(result.empty()) << "Expected empty result for no tokens after assignment operator";
}

TEST(StatementParser, UnexpectedTokenBetweenParenAndAssignment)
{
    MaybeError report;
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    std::deque<Token> rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::rparen, ")", 18),
        Token(Token::Type::id, "unexpected", 19),
        Token(Token::Type::op, ":=", 20),
        Token(Token::Type::id, "myDoubleVar", 22),
        Token(Token::Type::eof, "", 30)
    };

    std::deque<Token> result = statementPUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for unexpected token between parenthesis and assignment";
    ASSERT_TRUE(result.empty()) << "Expected empty result for unexpected token between parenthesis and assignment";
}

TEST(StatementParser, LineParser)
{

    struct TestCase
    {
        const std::string line;
        const int length_statement_begin;
    };

    std::vector<TestCase> valid_cases = {
        {"myVariableDate myVar1 += myVar2",                                        3},
        {"myVariableDate PAY(enum_1, enum_2) := myDoubleVar",                      2},
        {"myVariableDate PAY_AT (dateVar, enum_1, enum_2, enum_3) := myDoubleVar", 2},
        {"myVariableDate IF myExpression",                                         2},
        {"myVariableDate IF MAX(1, 2, 3) > 2",                                     2},
        {"CONTINUOUS IF myExpression",                                             2},
        {"CONTINUOUS IF MAX(1, 2, 3) > 2",                                         2},
        {"CONTINUOUS myVar1 += myVar2",                                            3},
        {"    myVar1 **= myVar2",                                                  3},
        {"    PAY(enum_1, enum_2) := myDoubleVar",                                 2},
        {"    PAY_AT (dateVar, enum_1, enum_2, enum_3) := myDoubleVar",            2},
        {"    TERMINATE",                                                          2},
    };

    std::vector<std::vector<std::string>> expression_infix_expected = {
        {"myVar2"},
        {"PAY",    "(", "enum_1",  ",", "enum_2", ",", "myDoubleVar", ")"},
        {"PAY_AT", "(", "dateVar", ",", "enum_1", ",", "enum_2",      ",", "enum_3", ",", "myDoubleVar", ")"},
        {"myExpression"},
        {"MAX",    "(", "1",       ",", "2",      ",", "3",           ")", ">",      "2"},
        {"myExpression"},
        {"MAX",    "(", "1",       ",", "2",      ",", "3",           ")", ">",      "2"},
        {"myVar2"},
        {"myVar2"},
        {"PAY",    "(", "enum_1",  ",", "enum_2", ",", "myDoubleVar", ")"},
        {"PAY_AT", "(", "dateVar", ",", "enum_1", ",", "enum_2",      ",", "enum_3", ",", "myDoubleVar", ")"},
        {}
    };

    assert(valid_cases.size() == expression_infix_expected.size());

    Lexer l;
    for (const auto &[v_case, e_expected]: std::ranges::zip_view(valid_cases, expression_infix_expected))
    {
        std::deque<Token> line_infix = l.tokenize(v_case.line);

        auto [report, result] = parseLine(line_infix);

        ASSERT_FALSE(report.isError()) << "Expected no error for valid line: " << v_case.line;
        ASSERT_EQ(result.statement_begin.size(), v_case.length_statement_begin)
                            << "Statement begin size mismatch for line: " << v_case.line;

        auto is_not_space = [](const Token &t)
        { return t.type != Token::Type::wsp && t.type != Token::Type::tab; };
        auto expression = result.expression_infix | std::ranges::views::filter(is_not_space);
        ASSERT_FALSE(expression.empty()) << "Expected non-empty expression for line: " << v_case.line;
        auto expr_copy = std::vector<Token>(expression.begin(), expression.end());
        expr_copy.pop_back(); // Remove the eof token
        ASSERT_EQ(e_expected.size(), expr_copy.size()) << "Expression size mismatch for line: " << v_case.line;
        for (const auto &[res, exp]: std::ranges::zip_view(expr_copy, e_expected))
        {
            EXPECT_EQ(res.value, exp) << "Expression value mismatch for line: " << v_case.line;
        }
    }

}