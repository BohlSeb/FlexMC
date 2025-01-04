#include <gtest/gtest.h>

#include <ranges>

#include "lexer.h"
#include "assignment_parser.h"


TEST(StatementParser, AssignmentParser)
{

    using namespace flexMC;

    struct TestCase
    {
        const int n_tokens;
        const std::string infix;
    };

    // Test cases from chat gbt

    std::vector<TestCase> valid_cases = {
        {3, "myDate myVar := "},
        {3, "myDate PAY := "},
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
        {3, "    PAY := "},
        {2, "    TERMINATE "},
    };

    std::vector<TestCase> bad_cases = {
        {3, "myDate myVar myVar := "},
        {3, "myDate := "},
        {3, "PAY myDate PAY := "},
        {3, "myDate CONTINUOUS"},
        {3, "BAD_SYMBOL myVar *= "},
        {3, "        myVar := "},
        {3, "TERMINATE"},
        {3, "IF xxx"},
    };

    Lexer lexer;

    for (const auto &c: valid_cases)
    {

        const std::deque<Token> tokens = lexer.tokenize(c.infix);
        const auto [parse_report, tokens_front] = parseLineStart(tokens);
        EXPECT_FALSE(parse_report.isError());
        EXPECT_EQ(tokens_front.size(), c.n_tokens);

    }

    for (const auto &c: bad_cases)
    {

        const std::deque<Token> tokens = lexer.tokenize(c.infix);
        const auto [parse_report, tokens_front] = parseLineStart(tokens);
        EXPECT_TRUE(parse_report.isError());
        std::cout << printError("Assignment Parser", c.infix, parse_report) << "\n";

    }

}