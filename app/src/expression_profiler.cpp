#include <cassert>
#include <cmath>
#include <iostream>

#include "expression_profiler.h"
#include "lexer.h"
#include "expression_parser.h"
#include "expression_compiler.h"

using namespace flexMC;


//void areEqualVector(const std::vector<double>& expected, const std::vector<double>& result, const double& precision) {
//	auto res = result.cbegin();
//	for (double it : expected) {
//		std::cout << "problem" << std::endl;
//		assert(std::fabs(it -  *res) < precision);
//		++res;
//	}
//}


void scalarOperations() {

	// Test cases against python console

	// Scalar tests

	std::cout << "scalar operations..." << std::endl;

	struct TestCase {
		std::string infix;
		const double result;
	};

	TestCase TestData[] = {
		{"2 + 3", 5.0},
		{"4 * 5 - 7", 13.0},
		{"8 + 9 * 10", 98.0},
		{"3 * (5 + 2)", 21.0},
		{"(4 + 6) * 8", 80.0},
		{"2 * (3 + 4)", 14.0},
		{"6 * 7 + 8", 50.0},
		{"9 * 10 - 11", 79.0},
		{"12 + 13 * (14 - 15) / 16", 11.1875},
		{"((2 + 3) * 4 - 5) / 6", 2.5},
		{"3 + 4 * 5", 23.0},
		{"6 * 7 + 8", 50.0},
		{"2**3 + 4**5", 1032},
		{"5**(2+3)", 3125.0},
		{"3 + 4**2 * 5", 83.0},
		{"2 * 3**4 - 5", 157},
		{"(3 + 4)**2", 49.0},
		{"5**(2 + 3) / 6", 520.8333333333334},
		{"3 + 4 * 5 / (2 - 1)", 23.0},
		{"(5 * (3 + 4)) ** 2 - 8 / 4", 1223.0},
		{"((9 - 3) * (4 + 7) / 2) ** 2 + 10", 1099.0},
		{"(12 / 3) * (7 - (4 + 1)) ** 2 + 10 / 2", 21.0},
		{"2 + (5 * 4) / (3 - 1) ** 2", 7.0},
		{"(6 * 3) / (5 - 2) ** 2 + (8 - 2) * 3", 20.0},
		{"(2 * 3 ** (4 - 2)) / (5 - 2) + 10", 16.0},
		{"(8 ** 2) / ((5 - 2) * (3 + 1)) - 7 + 2", 0.33333333333333304},
	};

    Lexer lexer;

	for (auto& c : TestData) {

        std::deque<Token> infix = lexer.tokenize(c.infix);
		std::pair<MaybeError, std::vector<Token>> parse_result = infixToPostfix(infix);
		auto parse_report = std::get<0>(parse_result);
        assert(!parse_report.isError());
		auto postfix = std::get<1>(parse_result);

		Expression expression;
		const std::pair<MaybeError, CompileReport> reports = compileExpression(postfix, expression);
        assert(!reports.first.isError());
        CalcStacks c_stacks(reports.second.max_scalar, reports.second.max_vector, 0, 0);


		int trys = 10;
		// double average = 0.0;
		for (int i = 0; i < trys; ++i) {
			expression.evaluate(c_stacks);
			// average += c_stacks.scalarsBack() / trys;
			c_stacks.popScalar();
            assert(c_stacks.ready());
		}
	}

	//// Vector Tests

	//using vec = std::vector<double>;



	//struct TestCase {
	//	std::string infix;
	//	const vec result;
	//};

	//TestCase TestData[] = {
	//	{"[2, 2] + [3, 2]", vec({5.0, 4.0})},
	//	{"[2, 2] / [2, 2]", vec({1.0, 1.0})},
	//	{"[2, 2] * [2, 2]", vec({4.0, 4.0})},
	//	{"[-2, -2] + [2, 2]", vec({0.0, 0.0})},
	//	{"[-2, -2] - [2, 2]", vec({-4.0, -4.0})},
	//	{"EXP(LOG([2, 2]))", vec({2.0, 2.0})},
	//	{"ABS(-1 * [4, 4, 4, 4, 4])", vec({4, 4, 4, 4, 4})},
	//	{"SQUARE([-1, 1, 2, 2])", vec({1, 1, 4, 4})},
	//	{"-SQRT([4, 4, 4, 4])", vec({-2, -2, -2, -2})},
	//	{"ABS(-1 * [4, 4, 4, 4, 4])", vec({4, 4, 4, 4, 4})},
	//	{"[2, 1] * (3 + 4)", vec({14.0, 7.0})},
	//	{"2**[3, 3] + 4**5", vec({1032, 1032})},
	//	{"3 + 4 * 5 / (EXP(LOG([2, 2] - [1, 1])))", vec({23.0, 23.0})},
	//};

	//ExpressionCompiler compiler;

	//for (auto& c : TestData) {

	//	Lexer lexer = Lexer(c.infix);
	//	ExpressionParser parser = ExpressionParser(lexer);
	//	std::deque<Token> parsed = parser.parseLine();

	//	Expression expression;
	//	auto report = compiler.compile(parsed, expression);
	//	CalcStacks c_stacks(0, 0, 0, 0);
	//	// CalcStacks c_stacks(report.max_scalar, report.max_vector, 0, 0);
	//	expression.evaluate(c_stacks);
	//	vec result = c_stacks.vectorsBack();
	//	c_stacks.popVector();
	//	assert(c_stacks.ready());
	//	areEqualVector(c.result, result, 1e-14);

	//	int trys = 10000;
	//	vec result_2 = vec(result.size(), 0.0);
	//	for (int i = 0; i < trys; ++i) {
	//		expression.evaluate(c_stacks);
	//		std::transform(result_2.cbegin(), result_2.cend(), c.result.cbegin(), result_2.begin(), std::plus<double>());
	//		c_stacks.popVector();
	//		assert(c_stacks.ready());
	//	}
	//	std::transform(result_2.cbegin(), result_2.cend(), result_2.begin(), [=](double v) {return v / trys; });
	//	areEqualVector(c.result, result_2, 1e-10);
	//}

}