// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <regex>
#include <unordered_map>

#include "lexer.h"
#include "language_constants.h"

using namespace flexMC;

int main() {

	Token::Type tokenType = Token::Type::eof;
	std::string value = "5";
	int position = 12;
	int line = 1;

	Token token = Token(tokenType, value, position, line);

	Token token2 = Token(Token::Type::eol, "this", position, line);

	std::string tokenDes = token2.toString();

	//std::cout << token.toString() << std::endl;
	//std::cout << tokenDes << std::endl;

	//tokenDes = "overwrite";
	//std::cout << tokenDes << std::endl;

	//std::cout << IF << std::endl;

	std::cout << "REGEX:" << std::endl;

	const char testString[] = "MIN ARGMIN MIN MIN ARGMIN MINARG ARGMIN ARGMIN MIN";
	std::cout << "Test string: " << testString << std::endl;

	std::cmatch matchObj;

	std::string expr1 = R"(ARGMIN)";
	std::string expr2 = R"(MIN)";
	std::string expr3 = R"(IF)";

	std::string combined = "(" + expr1 + ")|(" + expr2 + ")|(" + expr3 + ")";

	std::string standard = R"((x)|(ELSE)|(IF))";

	bool equals = (standard == combined);

	std::cout << standard << "\t" << combined << "\t" << std::boolalpha << equals << std::endl;

	//std::regex expression(R"((\+)|(IF)|(ELSE))");
	std::regex expression(combined);

	std::regex_search(testString, matchObj, expression);

	bool isEmpty = matchObj.empty();

	std::cout << "Match empty: " << std::boolalpha << isEmpty << std::endl;

	std::cout << "MATCH SIZE: " << matchObj.size() << std::endl;

	if (!isEmpty) {
		std::cout << "MATCH: " << matchObj.str() << std::endl;
		std::cout << "SUFFIX: " << matchObj.suffix() << std::endl;
		std::cout << "PREFIX: " << matchObj.prefix() << std::endl;
	}

	std::cout << "MATCH ITEMS:" << std::endl;

	for (int i = 0; i < matchObj.size(); ++i) {
		std::cout << "at: " << i << " " << matchObj[i] << " matched: " << matchObj[i].matched << std::endl;
	}

	std::string searchThrough = testString;
	std::smatch smatch;

	std::cout << "ALL MATCHES:" << "\n" << std::endl;
	std::cout << testString << "\n" << std::endl;

	while (std::regex_search(searchThrough, smatch, expression)) {
		std::cout << "MATCH: " << smatch.str() << std::endl;
		searchThrough = smatch.suffix();
	}

	double numtest = (double)1E6;
	std::cout << numtest << std::endl;

	std::cout << "\n" << "UNORDERD MAP TEST:" << std::endl;

	const std::unordered_map<std::string, Token> testMap = {
		{"IF", token},
		{"ELSE", token2}
	};

	for (auto i = testMap.begin(); i != testMap.end(); i++) {
		Token newToken = i->second;
		std::cout << "key: " << i->first << " value: " << newToken.toString() << std::endl;
	}

	std::cout << "LOOK UP IF" << std::endl;

	std::string key = "IFF";

	std::unordered_map<std::string, Token>::const_iterator lookUp = testMap.find(key);

	if (lookUp == testMap.end()) {
		std::cout << "Not found: " << key << std::endl;
	}
	else {
		Token value = lookUp->second;
		std::cout << "Key: " << key << "\tValue: " << value.toString() << std::endl;
	};

	std::cout << "\n" << "LEXER TEST:" << std::endl;

	const std::string program = "LEXER TEST PROGRAM";

	Lexer lexer = Lexer(program);

	lexer.reset();

	const int iters = 3;

	for (int i = 0; i < iters; ++i) {
		Token newToken = lexer.nextToken();
		std::cout << "Iter: " << i << " New token: " << newToken.toString() << std::endl;
	}
	

	return 0;
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
