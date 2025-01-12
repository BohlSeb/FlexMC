#include <string>
#include <utility>
#include <cassert>
#include <ranges>
#include <benchmark/benchmark.h>

#include <iostream>

#include "lexer.h"
#include "expression_parser.h"
#include "expression_compiler.h"
#include "expression_stacks.h"


using namespace flexMC;


// Learn about inheritance in c++ to reduce code duplication


CompileReport parseExpressions(const std::vector<std::string> &str_expressions,
                               std::vector<Expression> &expressions,
                               StaticVStorage &s_variables) {
    assert(expressions.size() == str_expressions.size());
    Lexer l;
    std::size_t max_s{0};
    std::size_t max_v{0};
    for (const auto &[exp_str, exp]: std::ranges::zip_view(str_expressions, expressions)) {
        const auto [report, postfix] = infixToPostfix(l.tokenize(exp_str));
        if (report.isError()) {
            std::cout << printError("Expression Parser", exp_str, report) << "\n";
        }
        assert(!report.isError());
        const auto [e_report, c_report] = compileExpression(postfix, exp, s_variables);
        if (e_report.isError()) {
            std::cout << printError("Expression Compiler", exp_str, e_report) << "\n";
        }
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


const std::vector<std::string> SCALAR_S_VARIABLES = {
    "(x * y*LOG(EXP(-x))) / -(a - x) + e - SQRT(z) - SQUARE(x)",
    "((d - y) * (z + b) / x) / x + 10 + LOG(x) + 1 / EXP(one)",
    "x + -ABS(-(a * z)) / (y - 1)*x",
    "(c * +x) / ((a - +x) * (y + 1)) - b + x",
    "(1000 / y) * (b - (z + 1)) * x + e / x",
};


const std::vector<std::string> VECTOR_EXPRESSIONS = {
    "ABS((2, 2) / ((2, 2) - (4, 4))) * (1, 1) / (1, 1) + (-1, 2) - (2, -1)",
    "(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20) / (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)",
    "SQUARE(SQRT(ABS(-1 * LOG(EXP((1, 2, 3, 4))))))",
    "3 + 4 * 5 / EXP(LOG((2, 2) - (1, 1)))",
    " (5, 5) * ((-2, -2) - (2, 2) - (1, 1) + (1, 1) / (2, 2))",
};


const std::vector<std::string> VECTOR_S_VARIABLES = {
    "ABS((twos) / ((twos) - (fours))) * ones / ones + minus_one_two - two_minus_one",
    "longRange / (longRange + 1)",
    "SQUARE(SQRT(ABS(-1 * LOG(EXP((range))))))",
    "3 + 4 * 5 / EXP(LOG(twos - ones))",
    " fives * (-twos - twos - ones + ones / twos)",
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


static void BM_Scalars(benchmark::State &state) {
    std::vector<Expression> expressions(5);
    StaticVStorage s_variables;
    const auto report = parseExpressions(SCALAR_EXPRESSIONS, expressions, s_variables);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};

    for (auto _: state) {
        const std::size_t end = state.range(0);
        for (std::size_t i{0}; i < end; ++i) {
            for (const auto &exp: expressions) {
                exp(stacks);
                stacks.scalars().pop_back();
                assert(stacks.ready());
            }
        }
    }
}


static void BM_Vectors(benchmark::State &state) {
    std::vector<Expression> expressions(5);
    StaticVStorage s_variables;
    const auto report = parseExpressions(VECTOR_EXPRESSIONS, expressions, s_variables);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state) {
        const std::size_t end = state.range();
        for (std::size_t i{0}; i < end; ++i) {
            for (const auto &exp: expressions) {
                exp(stacks);
                stacks.popVectorResult();
                assert(stacks.ready());
            }
        }
    }
}


static void BM_ReduceScalars(benchmark::State &state) {
    std::vector<Expression> expressions(6);
    StaticVStorage s_variables;
    const auto report = parseExpressions(SCALAR_REDUCE_EXPRESSIONS, expressions, s_variables);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state) {
        const std::size_t end = state.range(0);
        for (std::size_t i{0}; i < end; ++i) {
            for (const auto &exp: expressions) {
                exp(stacks);
                stacks.scalars().pop_back();
                assert(stacks.ready());
            }
        }
    }
}


static void BM_ReduceVectors(benchmark::State &state) {
    std::vector<Expression> expressions(6);
    StaticVStorage s_variables;
    const auto report = parseExpressions(VECTOR_REDUCE_EXPRESSIONS, expressions, s_variables);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state) {
        const std::size_t end = state.range(0);
        for (std::size_t i{0}; i < end; ++i) {
            for (const auto &exp: expressions) {
                exp(stacks);
                stacks.scalars().pop_back();
                assert(stacks.ready());
            }
        }
    }
}


static void BM_StaticScalarVars(benchmark::State &state) {
    std::vector<Expression> expressions(5);
    StaticVStorage s_variables;
    s_variables.insert<SCALAR>("one", 1);
    s_variables.insert<SCALAR>("x", 2);
    s_variables.insert<SCALAR>("y", 3);
    s_variables.insert<SCALAR>("z", 4);
    s_variables.insert<SCALAR>("a", 5);
    s_variables.insert<SCALAR>("b", 7);
    s_variables.insert<SCALAR>("c", 8);
    s_variables.insert<SCALAR>("d", 9);
    s_variables.insert<SCALAR>("e", 10);
    const auto report = parseExpressions(SCALAR_S_VARIABLES, expressions, s_variables);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state) {
        const std::size_t end = state.range(0);
        for (std::size_t i{0}; i < end; ++i) {
            for (const auto &exp: expressions) {
                exp(stacks);
                stacks.scalars().pop_back();
                assert(stacks.ready());
            }
        }
    }
}


static void BM_StaticVectorVars(benchmark::State &state) {
    std::vector<Expression> expressions(5);
    StaticVStorage s_variables;
    s_variables.insert<VECTOR>("ones", {1, 1});
    s_variables.insert<VECTOR>("twos", {2, 2});
    s_variables.insert<VECTOR>("fours", {4, 4});
    s_variables.insert<VECTOR>("fives", {5, 5});
    s_variables.insert<VECTOR>("range", {1, 2, 3, 4});
    s_variables.insert<VECTOR>("minus_one_two", {-1, 2});
    s_variables.insert<VECTOR>("two_minus_one", {2, -1});

    //Python Console>>> c = list(range(1, 21))
    std::vector<SCALAR> c(20, 1);
    std::transform(c.begin(), c.end() - 1, c.begin() + 1,
                   [](const auto &previous) { return previous + 1; });
    s_variables.insert<VECTOR>("longRange", c);

    const auto report = parseExpressions(VECTOR_S_VARIABLES, expressions, s_variables);
    CalcStacks stacks{report.max_scalar, report.max_vector, 0, 0};
    for (auto _: state) {
        const std::size_t end = state.range(0);
        for (std::size_t i{0}; i < end; ++i) {
            for (const auto &exp: expressions) {
                exp(stacks);
                stacks.popVectorResult();
                assert(stacks.ready());
            }
        }
    }
}

BENCHMARK(BM_Scalars)->Arg(1);
BENCHMARK(BM_Vectors)->Arg(1);
BENCHMARK(BM_ReduceVectors)->Arg(1);
BENCHMARK(BM_ReduceScalars)->Arg(1);
BENCHMARK(BM_StaticScalarVars)->Arg(1);
BENCHMARK(BM_StaticVectorVars)->Arg(1);

BENCHMARK(BM_Scalars)->Unit(benchmark::kMillisecond)->Range(1 << 12, 1 << 20);
BENCHMARK(BM_Vectors)->Unit(benchmark::kMillisecond)->Range(1 << 12, 1 << 20);
BENCHMARK(BM_ReduceScalars)->Unit(benchmark::kMillisecond)->Range(1 << 12, 1 << 20);
BENCHMARK(BM_ReduceVectors)->Unit(benchmark::kMillisecond)->Range(1 << 12, 1 << 20);
BENCHMARK(BM_StaticScalarVars)->Unit(benchmark::kMillisecond)->Range(1 << 12, 1 << 20);
BENCHMARK(BM_StaticVectorVars)->Unit(benchmark::kMillisecond)->Range(1 << 12, 1 << 20);

BENCHMARK_MAIN();
