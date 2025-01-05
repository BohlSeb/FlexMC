#include <gtest/gtest.h>

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

    std::vector<std::string> bad_cases = {
        "myDate myVar myVar := ",
        "myDate := ",
        "PAY myDate PAY := ",
        "myDate CONTINUOUS",
        "BAD_SYMBOL myVar *= ",
        "myVar BAD_SYMBOL *= ",
        "        myVar := ",
        "TERMINATE",
        "IF xxx",
        "",
        "    ",
        "   ",
    };

    Lexer lexer;

    for (const auto &c: valid_cases)
    {

        const std::deque<Token> tokens = lexer.tokenize(c.infix);
        const auto [parse_report, tokens_front] = stripStartOfLine(tokens);
        EXPECT_FALSE(parse_report.isError());
        EXPECT_EQ(tokens_front.size(), c.n_tokens);

    }

    for (const auto &infix: bad_cases)
    {

        const std::deque<Token> tokens = lexer.tokenize(infix);
        const auto [parse_report, tokens_front] = stripStartOfLine(tokens);
        EXPECT_TRUE(parse_report.isError());
        std::cout << printError("Assignment Parser", infix, parse_report) << "\n";

    }

}