#include "expression_profiler.h"
#include "lexer.h"
#include "expression_parser.h"
#include "expression_compiler.h"

using namespace flexMC;


void scalarOperations() {

	struct TestCase {
		std::string infix;
		const double result;
	};

	// Test cases against python console

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

	for (auto& c : TestData) {

		Lexer lexer = Lexer(c.infix);
		ExpressionParser parser = ExpressionParser(lexer);
		std::deque<Token> parsed = parser.parseLine();
		ExpressionCompiler compiler = ExpressionCompiler(parsed);
		// Assert::AreEqual(compiler.resultType(), Operands::Type::scalar);

		std::vector<std::shared_ptr<PostFixItem>> main_l = compiler.compiled();
		CalcStacks c_stacks;

		int trys = 1;
		double average = 0.0;
		for (int i = 0; i < trys; ++i) {
			for (auto iter = main_l.cbegin(); iter != main_l.cend(); ++iter) {
				(*iter)->evaluate(c_stacks);
			}
			average += c_stacks.scalarsBack() / trys;
			c_stacks.popScalar();
		}

		trys = 10000;
		average = 0.0;
		for (int i = 0; i < trys; ++i) {
			for (auto iter = main_l.cbegin(); iter != main_l.cend(); ++iter) {
				(*iter)->evaluate(c_stacks);
			}
			average += c_stacks.scalarsBack() / trys;
			c_stacks.popScalar();
		}
	}

}