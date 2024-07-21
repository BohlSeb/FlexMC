#include <iostream>
#include <string>
#include <benchmark/benchmark.h>

#include "lexer.h"
#include "expression_parser.h"
#include "expression_compiler.h"
#include "expression_stacks.h"


using namespace flexMC;

class ExpressionFixture : public benchmark::Fixture
{

public:
    void SetUp(const benchmark::State &state) override
    {
        Lexer l;
        const std::deque<Token> infix = l.tokenize(str_expression);
        const auto [p_report, postfix] = infixToPostfix(infix);
        const auto [e_report, c_report] = compileExpression(postfix, expression);
        max_s = c_report.max_scalar;
        max_v = c_report.max_vector;
    }

    Expression expression;

    const std::string str_expression = "(2 * 3*LOG(EXP(-2))) / -(5 - 2) + 10 - SQRT(4) - SQUARE(2)";
    std::size_t max_s;
    std::size_t max_v;

};


BENCHMARK_DEFINE_F(ExpressionFixture, Test)(benchmark::State &state)
{
    CalcStacks stacks(max_s, max_v, 0, 0);
    for (auto _: state) {
        expression.evaluate(stacks);
        stacks.popScalar();
    }
}


//BENCHMARK_REGISTER_F(ExpressionFixture, Test)->Range(1 << 11, 1 << 15);
BENCHMARK_REGISTER_F(ExpressionFixture, Test)->Arg(1 << 11)->Arg(1 << 12)->Arg(1 << 13)->Arg(1 << 14)->Arg(1 << 15)->Arg(1 << 16);

//static void BM_stringComparison(benchmark::State &state)
//{
//    std::cout << "Setup" << std::endl;
//    for (auto _: state)
//    {
//        stringComparison(10000);
//    }
//}
//
//BENCHMARK(BM_stringComparison);

BENCHMARK_MAIN();
