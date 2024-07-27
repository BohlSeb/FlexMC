#include <string>
#include <utility>
#include <cassert>
#include <ranges>
#include <benchmark/benchmark.h>

#include "lexer.h"
#include "expression_parser.h"
#include "expression_compiler.h"
#include "expression_stacks.h"


using namespace flexMC;


// Learn about inheritance in c++ to reduce code duplication


CompileReport parseExpressions(const std::vector<std::string> &str_expressions, std::vector<Expression> &expressions)
{
    assert(expressions.size() == str_expressions.size());
    Lexer l;
    std::size_t max_s{0};
    std::size_t max_v{0};
    for (const auto &[exp_str, exp]: std::ranges::zip_view(str_expressions, expressions))
    {
        const auto [report, postfix] = infixToPostfix(l.tokenize(exp_str));
        assert(!report.isError());
        const auto [e_report, c_report] = compileExpression(postfix, exp);
        assert(!e_report.isError());
        max_s = std::max<>(max_s, c_report.max_scalar);
        max_v = std::max<>(max_v, c_report.max_vector);
    }
    CompileReport out_report{CType::scalar, max_s, max_v};
    return out_report;
}


const std::vector<std::string> SCALAR_EXPRESSIONS = {
        "(2 * 3*LOG(EXP(-2))) / -(5 - 2) + 10 - SQRT(4) - SQUARE(2)",
        "((9 - 3) * (4 + 7) / 2) / 2 + 10 + LOG(2) + 1 / EXP(1)",
        "2 + -ABS(-(5 * 4)) / (3 - 1)*2",
        "(8 * +2) / ((5 - +2) * (3 + 1)) - 7 + 2",
        "(1000 / 3) * (7 - (4 + 1)) * 2 + 10 / 2",
};


const std::vector<std::string> VECTOR_EXPRESSIONS = {
        "ABS((2, 2) / ((2, 2) - (4, 4))) * (1, 1) / (1, 1) + (-1, 2) - (2, -1)",
        "(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20) / (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)",
        "SQUARE(SQRT(ABS(-1 * LOG(EXP((1, 2, 3, 4))))))",
        "3 + 4 * 5 / EXP(LOG((2, 2) - (1, 1)))",
        " (5, 5) * ((-2, -2) - (2, 2) - (1, 1) + (1, 1) / (2, 2))",
};


const std::vector<std::string> SCALAR_REDUCE_EXPRESSIONS = {
        {"MAX(-2, 3, 4)"},
        {"MIN(-2, 3, 4)"},
        {"ARGMAX(-2, -3, -4, -3, 4)"},
        {"ARGMIN(2, 3, 1, 1, 4, 1, 3, 4)"},
        {"SUM(-2, -3, -4, -3, 4)"},
        {"PROD(-2, -3, 4, -3, 4)"},
};


const std::vector<std::string> VECTOR_REDUCE_EXPRESSIONS = {
        {"MAX((-2, 4, 3))"},
        {"MIN((-2, 4, 3))"},
        {"ARGMAX(ABS((-2, -3, -4, -3, 4)))"},
        {"ARGMIN((2, 3, 1, 1, 4, 1, 3, 4))"},
        {"SUM(ABS((-2, -3, -4, -3, 4)))"},
        {"PROD((-2, -3, 4, -3, 4))"},
};


static void BM_Scalars(benchmark::State &state)
{
    std::vector<Expression> expressions(5);
    const auto report = parseExpressions(SCALAR_EXPRESSIONS, expressions);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state)
    {
        for (const auto &exp: expressions)
        {
            exp.evaluate(stacks);
            auto result = stacks.scalarsBack();
            benchmark::DoNotOptimize(result);
            stacks.popScalar();
            assert(stacks.ready());
        }
    }
}


static void BM_Vectors(benchmark::State &state)
{
    std::vector<Expression> expressions(5);
    const auto report = parseExpressions(VECTOR_EXPRESSIONS, expressions);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state)
    {
        for (const auto &exp: expressions)
        {
            exp.evaluate(stacks);
            auto result = stacks.vectorsBack();
            benchmark::DoNotOptimize(result);
            stacks.popVector();
            assert(stacks.ready());
        }
    }
}


static void BM_ReduceScalars(benchmark::State &state)
{
    std::vector<Expression> expressions(6);
    const auto report = parseExpressions(SCALAR_REDUCE_EXPRESSIONS, expressions);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state)
    {
        for (const auto &exp: expressions)
        {
            exp.evaluate(stacks);
            auto result = stacks.scalarsBack();
            benchmark::DoNotOptimize(result);
            stacks.popScalar();
            assert(stacks.ready());
        }
    }
}


static void BM_ReduceVectors(benchmark::State &state)
{
    std::vector<Expression> expressions(6);
    const auto report = parseExpressions(VECTOR_REDUCE_EXPRESSIONS, expressions);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state)
    {
        for (const auto &exp: expressions)
        {
            exp.evaluate(stacks);
            auto result = stacks.scalarsBack();
            benchmark::DoNotOptimize(result);
            stacks.popScalar();
            assert(stacks.ready());
        }
    }
}


BENCHMARK(BM_Scalars)->Range(1 << 12, 1 << 20);


BENCHMARK(BM_Vectors)->Range(1 << 12, 1 << 20);


BENCHMARK(BM_ReduceScalars)->Range(1 << 12, 1 << 20);


BENCHMARK(BM_ReduceVectors)->Range(1 << 12, 1 << 20);


BENCHMARK_MAIN();
