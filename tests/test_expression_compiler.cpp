#include <gtest/gtest.h>

#include "lexer.h"
#include "expression_parser.h"
#include "expression_compiler.h"


using namespace flexMC;


TEST(ExpressionCompiler, RealOperatorsScalar)
{

    struct TestCase
    {
        std::string infix;
        const double result;
    };

    // Test cases against python console
    std::vector<TestCase> test_data = {
            {"2 + 3",                                    5.0},
            {"4 * 5 - 7",                                13.0},
            {"8 + 9 * 10",                               98.0},
            {"EXP(0.0) * 3 * (5 + 2)",                   21.0},
            {"(4 + 6) * 8",                              80.0},
            {"2 * (3 + 4)",                              14.0},
            {"6 * 7 + 8",                                50.0},
            {"9 * 10 - 11",                              79.0},
            {"12 + 13 * (14 - 15) / 16",                 11.1875},
            {"((2 + 3) * 4 - 5) / 6",                    2.5},
            {"3 + 4 * 5",                                23.0},
            {"6 * 7 + 8",                                50.0},
            {"2**3 + SQRT(16) **5",                      1032},
            {"5**(2+3)",                                 3125.0},
            {"3 + SQUARE(2)**2 * 5",                     83.0},
            {"2 * 3**4 - 5",                             157},
            {"(3 + 4)**2",                               49.0},
            {"5**(2 + 3) / 6",                           520.8333333333334},
            {"3 + 4 * 5 / (2 - 1)",                      23.0},
            {"+(5 * (3 + 4)) ** 2 - 8 / 4",              1223.0},
            {"((9 - 3) * (4 + 7) / 2) ** 2 + 10",        1099.0},
            {"(12 / 3) * (7 - (4 + 1)) ** 2 + 10 / 2",   21.0},
            {"2 + (5 * 4) / (3 - 1) ** 2",               7.0},
            {"(6 * 3) / (5 - 2) ** 2 + (8 - 2) * 3",     20.0},
            {"(2 * 3 ** (4 - 2)) / (5 - 2) + 10",        16.0},
            {"(8 ** +2) / ((5 - +2) * (3 + 1)) - 7 + 2", 0.33333333333333304},
            {"-(3 + 4) * (5 - 2) / 2**3",                -2.625},
            {"10 * -((5 - 2)**2 - 4) / 3",               -16.666666666666668},
            {"2**-3 - -5 * (4 - 2) + 8 / 4",             12.125},
            {"-(7 - 3)**2 / (5 - 2) + 4 * (9 - 2)",      22.666666666666668},
            {"2 + -ABS(-(5 * 4)) / (3 - 1)**2",          -3.0},
            {"(6 * 3) / (5 - 2)**-2 + -(8 - 2) * 3",     144.0},
            {"(2 * +3**LOG(EXP(-2))) / -(5 - 2) + +10",  9.925925925925926},
            {"(8**2) / -((5 - 2) * (3 + 1)) - 7 + 2",    -10.333333333333332},
            {"((9 - 3) * (4 + 7) / 2)**2 + -10",         1079.0},
            {"(12 / 3) * -(7 - (4 + 1))**2 + 10 / 2",    -11.0},
    };

    std::vector<Expression> expressions;
    std::size_t s_max = 0;
    std::size_t v_max = 0;

    Lexer lexer;
    StaticVStorage storage;

    for (const auto &c: test_data)
    {

        const std::deque<Token> infix = lexer.tokenize(c.infix);
        const auto [parse_report, postfix] = infixToPostfix(infix);

        EXPECT_FALSE(parse_report.isError());

        Expression expression;
        const auto [error_report, compile_report] = compileExpression(postfix, expression, storage);
        ASSERT_FALSE(error_report.isError());
        CalcStacks c_stacks(compile_report.max_scalar, compile_report.max_vector, 0, 0);
        expression(c_stacks);

        const double result = c_stacks.scalarsBack();
        c_stacks.popScalar();
        ASSERT_TRUE(c_stacks.ready());
        EXPECT_DOUBLE_EQ(c.result, result);

        s_max = std::max<size_t>(s_max, compile_report.max_scalar);
        v_max = std::max<size_t>(v_max, compile_report.max_vector);

        expressions.push_back(std::move(expression));
    }


    int trys = 10000;
    CalcStacks c_stacks(s_max, v_max, 0, 0);

    for (int i = 0; i < trys; ++i)
    {
        for (const auto &expression: expressions)
        {
            expression(c_stacks);
            c_stacks.popScalar();
            ASSERT_TRUE(c_stacks.ready());
        }
    }

}

void areEqualVector(const std::vector<double> &expected, const std::vector<double> &result)
{
    auto res = result.cbegin();
    for (const double it: expected)
    {
        EXPECT_DOUBLE_EQ(it, *res);
        ++res;
    }
}

TEST(ExpressionCompiler, RealOperatorsVector)
{
    using vec = std::vector<double>;
    struct TestCase
    {
        std::string infix;
        const vec result;
    };

    std::vector<TestCase> test_data = {
            {"(2, 2) + (3, 2)",                         vec({5.0, 4.0})},
            {"(2, 2) / (2, 2)",                         vec({1.0, 1.0})},
            {"(2, 2) * (2, 2)",                         vec({4.0, 4.0})},
            {"((2, 2) + (3, 2)) * (2, 2)",              vec({10.0, 8.0})},
            {"ABS((2, 2) / ((2, 2) - (4, 4)))",         vec({1.0, 1.0})},
            {"LOG((2, 2) * (1/2, 1/2))",                vec({0.0, 0.0})},
            {"(-2, -2) + (2, 2) - (1, 1)",              vec({-1.0, -1.0})},
            {"(-2, -2) - (2, 2) - (1, 1)",              vec({-5.0, -5.0})},
            {"EXP(LOG((2, 2)))",                        vec({2.0, 2.0})},
            {"ABS(-1 * (4, 4, 4, 4, 4))",               vec({4, 4, 4, 4, 4})},
            {"SQUARE((-1, 1, 2, 2))",                   vec({1, 1, 4, 4})},
            {"-SQRT((4, 4, 4, 4))",                     vec({-2, -2, -2, -2})},
            {"ABS(-1 * (4, 4, 4, 4, 4))",               vec({4, 4, 4, 4, 4})},
            {"(2, 1) * (3 + 4)",                        vec({14.0, 7.0})},
            {"2**(3, 3) + 4**5",                        vec({1032, 1032})},
            {"3 + 4 * 5 / (EXP(LOG((2, 2) - (1, 1))))", vec({23.0, 23.0})},
    };

    std::vector<Expression> expressions;
    std::size_t s_max = 0;
    std::size_t v_max = 0;

    Lexer lexer;
    StaticVStorage storage;

    for (const auto &c: test_data)
    {

        const std::deque<Token> infix = lexer.tokenize(c.infix);
        const auto [parse_report, postfix] = infixToPostfix(infix);

        EXPECT_FALSE(parse_report.isError());

        Expression expression;
        const auto [error_report, compile_report] = compileExpression(postfix, expression, storage);

        ASSERT_FALSE(error_report.isError());

        CalcStacks c_stacks(compile_report.max_scalar, compile_report.max_vector, 0, 0);
        expression(c_stacks);

        vec result = c_stacks.vectorsBack();
        c_stacks.popVector();

        ASSERT_TRUE(c_stacks.ready());

        areEqualVector(c.result, result);

        s_max = std::max<size_t>(s_max, compile_report.max_scalar);
        v_max = std::max<size_t>(v_max, compile_report.max_vector);

        expressions.push_back(std::move(expression));
    }

    int trys = 10000;
    CalcStacks c_stacks(s_max, v_max, 0, 0);

    for (int i = 0; i < trys; ++i)
    {
        for (const auto &expression: expressions)
        {
            expression(c_stacks);
            c_stacks.popVector();
            ASSERT_TRUE(c_stacks.ready());
        }
    }

}

TEST(ExpressionCompiler, RealOperatorsReduce)
{

    struct TestCase
    {
        std::string infix;
        const double result;
    };

    std::vector<TestCase> test_data = {
            {"2 * MAX(-2, 3, 4) + 1",                    9},
            {"2 * MAX((-2, 4, 3)) + 1",                  9},
            {"2 * MIN(-2, 3, 4) + 1",                    -3},
            {"2 * MIN((-2, 3, 4)) + 1",                  -3},
            {"2 * ARGMAX(2, 3, 4, 3, 4) + 1",            5},
            {"2 * ARGMAX(ABS((-2, -3, -4, -3, 4))) + 1", 5},
            {"2 * ARGMIN(2, 3, 1, 1, 4, 1, 3, 4) + 1",   5},
            {"2 * ARGMIN((-2, -3, -4, -3, 4, -4)) + 1",  5},
            {"2 * SUM(2, 3, 4, 3, 4) + 1",               33},
            {"2 * SUM(ABS((-2, -3, -4, -3, 4))) + 1",    33},
            {"2 * PROD(2, 3, 4, 3, 4) + 1",              577},
            {"2 * PROD((-2, -3, 4, -3, 4)) + 1",         -575},
            {"2 * LEN((1,1,1,1)) + 1",                   9},
            {"2 * LEN((2, 1)) + 1",                      5},
    };

    std::vector<Expression> expressions;
    std::size_t s_max = 0;
    std::size_t v_max = 0;

    Lexer lexer;
    StaticVStorage storage;

    for (const auto &c: test_data)
    {

        const std::deque<Token> infix = lexer.tokenize(c.infix);
        const auto [parse_report, postfix] = infixToPostfix(infix);

        EXPECT_FALSE(parse_report.isError());

        Expression expression;
        const auto [error_report, compile_report] = compileExpression(postfix, expression, storage);

        ASSERT_FALSE(error_report.isError());

        CalcStacks c_stacks(compile_report.max_scalar, compile_report.max_vector, 0, 0);
        expression(c_stacks);

        const double result = c_stacks.scalarsBack();
        c_stacks.popScalar();
        ASSERT_TRUE(c_stacks.ready());
        EXPECT_DOUBLE_EQ(c.result, result);

        s_max = std::max<size_t>(s_max, compile_report.max_scalar);
        v_max = std::max<size_t>(v_max, compile_report.max_vector);

        expressions.push_back(std::move(expression));
    }


    int trys = 10000;
    CalcStacks c_stacks(s_max, v_max, 0, 0);

    for (int i = 0; i < trys; ++i)
    {
        for (const auto &expression: expressions)
        {
            expression(c_stacks);
            c_stacks.popScalar();
            ASSERT_TRUE(c_stacks.ready());
        }
    }

}


TEST(ExpressionCompiler, ScalarStaticVariables)
{

    struct TestCase
    {
        std::string infix;
        const double result;
    };

    std::vector<TestCase> test_data = {
            {"2 + x + y",                                     7.0},
            {"z * w - v",                                     13.0},
            {"a + b * c",                                     98.0},
            {"EXP(e) * y * (w + x)",                          21.0},
            {"(z + u) * a / a",                               10.0},
            {"x * (y + z)",                                   14.0},
            {"u * v + a",                                     50.0},
            {"b * c - d",                                     79.0},
            {"2 * MAX(-x, y, z) + 1",                         9},
            {"2 * MAX((-x, y, z)) + 1",                       9},
            {"2 * MIN(performances / 1) + 1",                 -3},
            {"2 * ARGMAX( (x, y, z, y, z) * 3 + 1) + 1",      5},
            {"2 * ARGMAX(SQUARE(ABS(basketValues)) / 1) + 1", 5},
            {"2 * ARGMIN(1 / basketValues) + 1",              1},
            {"2 * SUM(performances) + 1",                     11},
            {"2 * SUM(ABS((basketValues))) + 1",              33},
    };

    std::vector<Expression> expressions;
    std::size_t s_max = 0;
    std::size_t v_max = 0;

    StaticVStorage storage;
    storage.insert<SCALAR>("x", 2);
    storage.insert<SCALAR>("y", 3);
    storage.insert<SCALAR>("z", 4);
    storage.insert<SCALAR>("w", 5);
    storage.insert<SCALAR>("u", 6);
    storage.insert<SCALAR>("v", 7);
    storage.insert<SCALAR>("a", 8);
    storage.insert<SCALAR>("b", 9);
    storage.insert<SCALAR>("c", 10);
    storage.insert<SCALAR>("d", 11);
    storage.insert<SCALAR>("e", 0.0);
    storage.insert<VECTOR>("performances", {-2, 3, 4});
    storage.insert<VECTOR>("basketValues", {-2, -3, -4, -3, 4});


    Lexer lexer;

    for (const auto &c: test_data)
    {

        const std::deque<Token> infix = lexer.tokenize(c.infix);
        const auto [parse_report, postfix] = infixToPostfix(infix);

        EXPECT_FALSE(parse_report.isError());

        Expression expression;

        const auto [error_report, compile_report] = compileExpression(postfix, expression, storage);
        ASSERT_FALSE(error_report.isError());

        CalcStacks c_stacks(compile_report.max_scalar, compile_report.max_vector, 0, 0);
        expression(c_stacks);

        const double result = c_stacks.scalarsBack();
        c_stacks.popScalar();
        ASSERT_TRUE(c_stacks.ready());
        EXPECT_DOUBLE_EQ(c.result, result);

        s_max = std::max<size_t>(s_max, compile_report.max_scalar);
        v_max = std::max<size_t>(v_max, compile_report.max_vector);

        expressions.push_back(std::move(expression));
    }


    int trys = 10000;
    CalcStacks c_stacks(s_max, v_max, 0, 0);

    for (int i = 0; i < trys; ++i)
    {
        for (const auto &expression: expressions)
        {
            expression(c_stacks);
            c_stacks.popScalar();
            ASSERT_TRUE(c_stacks.ready());
        }
    }

}