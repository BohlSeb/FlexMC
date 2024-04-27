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

using namespace flexMC;

int main() {

	const std::string program = "1 - EXP([3, 3,3,3,3,3,3,3,3])";
	// const std::string program = "EXP ([0.0, 1.0, 2.0] ) - 1";
	std::cout << "Program to parse >>" << std::endl;
	std::cout << program << std::endl << std::endl;

	CalcStacks calc_stacks;
	calc_stacks.pushScalar(4.0);
	calc_stacks.pushScalar(2.0);

	std::string test = "-";
	bool res = test == flexMC::MINUS;
	std::cout << std::boolalpha << res << std::endl;

	//auto look_up = operatorsCalc::operators.find("-_S_S");
	//if (look_up != operatorsCalc::operators.end()) {
	//	std::cout << "ok" << std::endl;
	//	const std::function<void(CalcStacks&)> minus = look_up->second;
	//	minus(calc_stacks);
	//	double res = calc_stacks.scalarsBack();
	//	std::cout << res << std::endl;
	//	calc_stacks.popScalar();
	//	std::cout << std::boolalpha << calc_stacks.ready() << std::endl;
	//}

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

	//CalcStacks stacks;

	//stacks.scalars.push_back(0.0);

	//std::vector<double> vector{ 0.0, 1.0, -1.0 };

	//stacks.vectors.push_back(vector);

	//functionsScalar::binary::exp(stacks);

	//functionsScalar::binary::expVec(stacks);

	//for (auto it : stacks.scalars) {
	//	std::cout << it << std::endl;
	//}

	//std::vector<double> res = stacks.vectors.back();
	//std::cout << "ArrayNum res" << std::endl;

	//for (auto it : res) {
	//	std::cout << it << std::endl;
	//}

	//int x = 5;



	//const int trys = 25;
	//for (int i = 0; i < trys; ++i) {
	//	try {
	//		Token byParser = parser.getNext();
	//		std::cout << "Parsed token: " << byParser.toString() << std::endl;
	//	}

	//	catch (const std::runtime_error& error) {
	//		std::cout << error.what() << std::endl;
	//	}
	//}
	//std::cout << "Parsed:" << std::endl;
	//parser.printParsed();
	// 
	//for (auto i = 0; i < 10; i++) {
	//	Token next = lexer.nextToken();
	//	std::cout << next.toString() << std::endl;
	//}

	//Token finalT = lexer.nextToken();
	//std::cout << "Expected lexer state: end of file(eof)" << std::endl;
	//std::cout << "actual:" << finalT.toString() << std::endl;

	//std::vector<double> left{ 0.0, 2.0, 3.0 };
	//std::vector<double> right{ 2.0, 4.0, 6.0 };
	//double rightScalar = 2.0;
	//
	//std::vector<double> result;
	//result.reserve(left.size());
	//std::transform(left.cbegin(), left.cend(), right.cbegin(), std::back_inserter(result), std::plus<double>{});
	//
	//std::vector<double> vecScalarRes;
	//vecScalarRes.reserve(left.size());
	//std::transform(left.cbegin(), left.cend(), std::back_inserter(vecScalarRes), [&](double elem) {return elem + rightScalar; });

	//for (auto it : result) {
	//	std::cout << it << std::endl;
	//}


	return 0;
}
