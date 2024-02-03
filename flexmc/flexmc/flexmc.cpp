// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdexcept>
#include <algorithm>
#include <functional>

#include "operands.h"
#include "lexer.h"
#include "expression_parser.h"
#include "terminals.h"

using namespace flexMC;

int main() {

	const std::string program = " hallo * a + b^2 * c ";
	std::cout << "Program to parse:" << std::endl;
	std::cout << program << std::endl;

	Lexer lexer = Lexer(program);

	ExpressionParser parser = ExpressionParser(lexer);
	//std::deque<Token> parsed = parser.parseLine();
	using riter = std::deque<Token>::reverse_iterator;
	using iter = std::deque<Token>::iterator;
	try {
		std::deque<Token> parsed = parser.parseLine();
		std::cout << "Items parsed: " << parsed.size() << std::endl;
	}
	catch (const std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		//throw error;
	}
	

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
