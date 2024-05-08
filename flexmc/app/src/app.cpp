// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdexcept>
#include <algorithm>
#include <functional>
#include <numeric>

#include "lexer.h"
#include "expression_parser.h"
#include "terminals.h"

#include "functions_real.h"
#include "expression_stacks.h"
#include "expression_compiler.h"
#include "operators_calc.h"

#include "expression_profiler.h"

using namespace flexMC;

int main() {

	scalarOperations();

	const bool run_main = 0;

	//struct Test {
	//	Test(int x, int y) : x(x), y(y) {}
	//	int x;
	//	int y;
	//};

	//Test e();

	//std::vector<int> v({ 1, 2, 3, 4 });
	//int i = 0;
	//const int size = v.size();
	//while (i++ < size) {
	//	std::cout << i << std::endl;
	//	std::cout << v.back() << std::endl;
	//	v.pop_back();
	//}
	//std::cout << "Size: " << v.size() << std::endl;

	//CalcStacks stacks;
	//std::vector<double> values;
	//values.push_back(-1.0);
	//values.push_back(1.0);
	//values.push_back(2.0);
	//values.push_back(4.0);
	//
	//stacks.pushVector(values);

	//std::function<void(CalcStacks&)> fun = functionsReal::reduceVec("LEN");
	//fun(stacks);
	//std::cout << "Result: " << stacks.scalarsBack() << std::endl;
	//stacks.popScalar();
	//std::cout << "Ready: " << std::boolalpha << stacks.ready() << std::endl;

	if (run_main) {

		const std::string program = "2 * SUM(ABS([-2, -3, -4, -3, 4])) + 1";
		// const std::string program = "EXP ([0.0, 1.0, 2.0] ) - 1";
		std::cout << "Program to parse >>" << std::endl;
		std::cout << program << std::endl << std::endl;

		Lexer lexer = Lexer(program);

		ExpressionParser parser = ExpressionParser(lexer);

		try {
			std::deque<Token> parsed = parser.parseLine();
			std::cout << "Items parsed: " << parsed.size() << std::endl;
			for (auto iter = parsed.rbegin(); iter != parsed.rend(); ++iter) {
				std::cout << (*iter).value << " ";
			}
			std::cout << std::endl;
			ExpressionCompiler compiler;
			Expression expression;
			auto report = compiler.compile(parsed, expression);
			std::cout << "flexmc compiled" << std::endl;

			Operands::Type return_type = report.ret_type;
			std::cout << "Return type: " << return_type << std::endl;

			CalcStacks c_stacks(0,0,0,0);

			expression.evaluate(c_stacks);
			if (c_stacks.size(Operands::Type::scalar) == 1) {
				std::cout << "Double result: " << c_stacks.scalarsBack() << std::endl;
				c_stacks.popScalar();
			}
			else if (c_stacks.size(Operands::Type::vector) == 1) {
				std::cout << "Vector result:" << std::endl << "[ ";
				std::vector<double> res = c_stacks.vectorsBack();
				for (auto r : res) {
					std::cout << r << ", ";
				}
				std::cout << "]" << std::endl;
				c_stacks.popVector();
			}
			else {
				throw std::runtime_error("No result ready");
			}
			if (!c_stacks.ready()) {
				throw std::runtime_error("Stacks not ready for next run");
			}
			std::cout << "flexmc calculated" << std::endl;


		}
		catch (const std::runtime_error& error) {
			std::cout << "Error:" << std::endl;
			std::cout << error.what() << std::endl;
			//throw error;
		}

	}

	return 0;
}
