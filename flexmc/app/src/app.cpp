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

	// scalarOperations();

	const bool run_main = 1;

	if (run_main) {

		const std::string program = "(2 * 3) ** *[+1, 2] * - 1";
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
			ExpressionCompiler compiler = ExpressionCompiler(parsed);
			std::cout << "flexmc compiled" << std::endl;

			std::vector<std::shared_ptr<PostFixItem>> main_loop = compiler.compiled();
			Operands::Type return_type = compiler.resultType();
			std::cout << "Return type: " << return_type << std::endl;

			CalcStacks c_stacks;

			for (auto iter = main_loop.cbegin(); iter != main_loop.cend(); ++iter) {
				(*iter)->evaluate(c_stacks);
			}
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
