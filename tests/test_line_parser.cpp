#include <gtest/gtest.h>

#include <ranges>

#include "lexer.h"
#include "line_parser.h"

using namespace flexMC;

TEST(LineParser, StartOfLineStripper) {
    struct TestCase {
        const int n_tokens;
        const std::string infix;
    };

    std::vector<TestCase> valid_cases = {
        {3, "myDate myVar := EXP(0)"},
        {2, "myDate PAY() := (EXP(LOG(1)))"},
        {2, "myDate PAY () := 1 + 3 + myVar / 3"},
        {2, "myDate PAY_AT() := (1 + 3 + myVar / 3)"},
        {2, "myDate PAY_AT () := MAX(myVariables)"},
        {3, "myDate myVar += MIN((1 + 3 + myVar / 3), (1 + 3 + myVar / 3))"},
        {3, "myDate myVar -= x"},
        {3, "myDate myVar *= x"},
        {3, "myDate myVar /= x"},
        {3, "myDate myVar **= x"},
        {3, "CONTINUOUS myVar := x"},
        {3, "CONTINUOUS myVar += x"},
        {3, "CONTINUOUS myVar -= x"},
        {3, "CONTINUOUS myVar *= x"},
        {3, "CONTINUOUS myVar /= x"},
        {3, "CONTINUOUS myVar **= x"},
        {2, "myDate IF x"},
        {2, "CONTINUOUS IF x"},
        {3, "    myVar := x"},
        {3, "    myVar += x"},
        {3, "    myVar -= x"},
        {3, "    myVar *= x"},
        {3, "    myVar /= x"},
        {3, "    myVar **= x"},
        {2, "    PAY () := x "},
        {2, "    PAY() := x "},
        {2, "    PAY_AT () := x "},
        {2, "    PAY_AT() := x "},
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

    for (const auto &c: valid_cases) {
        std::deque<Token> tokens = lexer.tokenize(c.infix);
        const auto [parse_report, parse_result] = parseStartOfLine(tokens);
        EXPECT_FALSE(parse_report.isError()) << "Expected no error for valid case: " << c.infix;
        EXPECT_EQ(parse_result.line_begin.size(), c.n_tokens) << "Token size mismatch for valid case: " << c.infix;
    }

    for (const auto &infix: bad_cases) {
        std::deque<Token> tokens = lexer.tokenize(infix);
        const auto [parse_report, _] = parseStartOfLine(tokens);
        EXPECT_TRUE(parse_report.isError()) << "Expected error for bad case: " << infix;
    }
}

TEST(LineParser, MakePaymentExpressionValid) {
    std::deque<Token> start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    const std::deque rest_of_line = {
        Token(Token::Type::id, "enum_1", 0),
        Token(Token::Type::op, ",", 0),
        Token(Token::Type::id, "enum_2", 0),
        Token(Token::Type::rparen, ")", 0),
        Token(Token::Type::op, ":=", 0),
        Token(Token::Type::id, "myDoubleVar", 0),
        Token(Token::Type::eof, "", 0)
    };

    const std::deque expected = {
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

    const std::deque<Token> result = lineParseUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_FALSE(report.isError()) << "Expected no error for valid payment expression";
    ASSERT_EQ(result.size(), expected.size()) << "Token size mismatch for valid payment expression";
    for (const auto [exp, tok]: std::ranges::zip_view(expected, result)) {
        EXPECT_EQ(tok.type, exp.type) << "Token type mismatch";
        EXPECT_EQ(tok.value, exp.value) << "Token value mismatch";
    }
}

TEST(LineParser, PaymentMissingClosingParenthesis) {
    MaybeError report;
    std::deque start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    const std::deque rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::op, ":=", 18),
        Token(Token::Type::id, "myDoubleVar", 21),
        Token(Token::Type::eof, "", 22)
    };

    const std::deque<Token> result = lineParseUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for missing closing parenthesis";
}

TEST(LineParser, EmptyRestOfLine) {
    MaybeError report;
    std::deque start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    const std::deque<Token> rest_of_line = {};

    const std::deque<Token> result = lineParseUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for empty rest of line";
    ASSERT_TRUE(result.empty()) << "Expected empty result for empty rest of line";
}

TEST(LineParser, NoAssignmentOperator) {
    MaybeError report;
    std::deque start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    const std::deque rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::rparen, ")", 18),
        Token(Token::Type::eof, "", 19)
    };

    const std::deque<Token> result = lineParseUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for missing assignment operator";
    ASSERT_TRUE(result.empty()) << "Expected empty result for missing assignment operator";
}

TEST(LineParser, NoTokensAfterAssignmentOperator) {
    MaybeError report;
    std::deque start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    const std::deque rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::rparen, ")", 18),
        Token(Token::Type::op, ":=", 20),
        Token(Token::Type::eof, "", 22)
    };

    std::deque<Token> result = lineParseUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for no tokens after assignment operator";
    ASSERT_TRUE(result.empty()) << "Expected empty result for no tokens after assignment operator";
}

TEST(LineParser, UnexpectedTokenBetweenParenAndAssignment) {
    MaybeError report;
    std::deque start_of_line = {
        Token(Token::Type::id, "PAY", 0),
        Token(Token::Type::lparen, "(", 3)
    };
    const std::deque rest_of_line = {
        Token(Token::Type::id, "enum_1", 4),
        Token(Token::Type::op, ",", 10),
        Token(Token::Type::id, "enum_2", 12),
        Token(Token::Type::rparen, ")", 18),
        Token(Token::Type::id, "unexpected", 19),
        Token(Token::Type::op, ":=", 20),
        Token(Token::Type::id, "myDoubleVar", 22),
        Token(Token::Type::eof, "", 30)
    };

    const std::deque<Token> result = lineParseUtils::makePaymentExpression(report, start_of_line, rest_of_line);

    ASSERT_TRUE(report.isError()) << "Expected error for unexpected token between parenthesis and assignment";
    ASSERT_TRUE(result.empty()) << "Expected empty result for unexpected token between parenthesis and assignment";
}

TEST(LineParser, LineParser) {
    struct TestCase {
        const std::string line;
        const int length_line_begin;
    };

    std::vector<TestCase> valid_cases = {
        {"myVariableDate myVar1 += myVar2", 3},
        {"myVariableDate PAY(enum_1, enum_2) := myDoubleVar", 2},
        {"myVariableDate PAY_AT (dateVar, enum_1, enum_2, enum_3) := myDoubleVar", 2},
        {"myVariableDate IF myExpression", 2},
        {"myVariableDate IF MAX(1, 2, 3) > 2", 2},
        {"CONTINUOUS IF myExpression", 2},
        {"CONTINUOUS IF MAX(1, 2, 3) > 2", 2},
        {"CONTINUOUS myVar1 += myVar2", 3},
        {"    myVar1 **= myVar2", 3},
        {"    PAY(enum_1, enum_2) := myDoubleVar", 2},
        {"    PAY_AT (dateVar, enum_1, enum_2, enum_3) := myDoubleVar", 2},
        {"    TERMINATE", 2},
    };

    std::vector<std::vector<std::string> > expression_infix_expected = {
        {"myVar2"},
        {"PAY", "(", "enum_1", ",", "enum_2", ",", "myDoubleVar", ")"},
        {"PAY_AT", "(", "dateVar", ",", "enum_1", ",", "enum_2", ",", "enum_3", ",", "myDoubleVar", ")"},
        {"myExpression"},
        {"MAX", "(", "1", ",", "2", ",", "3", ")", ">", "2"},
        {"myExpression"},
        {"MAX", "(", "1", ",", "2", ",", "3", ")", ">", "2"},
        {"myVar2"},
        {"myVar2"},
        {"PAY", "(", "enum_1", ",", "enum_2", ",", "myDoubleVar", ")"},
        {"PAY_AT", "(", "dateVar", ",", "enum_1", ",", "enum_2", ",", "enum_3", ",", "myDoubleVar", ")"},
        {}
    };

    assert(valid_cases.size() == expression_infix_expected.size());

    Lexer l;
    for (const auto &[v_case, e_expected]: std::ranges::zip_view(valid_cases, expression_infix_expected)) {
        std::deque<Token> line_infix = l.tokenize(v_case.line);

        auto [report, result] = parseStartOfLine(line_infix);

        ASSERT_FALSE(report.isError()) << "Expected no error for valid line: " << v_case.line;
        ASSERT_EQ(result.line_begin.size(), v_case.length_line_begin)
                            << "Statement begin size mismatch for line: " << v_case.line;

        auto is_not_space = [](const Token &t) { return t.type != Token::Type::wsp && t.type != Token::Type::tab; };
        auto expression = result.expression_infix | std::ranges::views::filter(is_not_space);
        ASSERT_FALSE(expression.empty()) << "Expected non-empty expression for line: " << v_case.line;
        auto expr_copy = std::vector(expression.begin(), expression.end());
        expr_copy.pop_back(); // Remove the eof token
        ASSERT_EQ(e_expected.size(), expr_copy.size()) << "Expression size mismatch for line: " << v_case.line;
        for (const auto &[res, exp]: std::ranges::zip_view(expr_copy, e_expected)) {
            EXPECT_EQ(res.value, exp) << "Expression value mismatch for line: " << v_case.line;
        }
    }
}
