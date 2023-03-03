// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <regex>
#include <unordered_map>
#include <stdexcept>

#include "lexer.h"
#include "language_constants.h"

using namespace flexMC;

int main() {

	Token::Type tokenType = Token::Type::eof;
	std::string value = "5";
	int position = 12;
	int line = 1;

	Token token = Token(tokenType, value);

	Token token2 = Token(Token::Type::eol, "this");

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

	const std::string program = "_THIS userGeneratedCode will be hacked into different tokens, keywords are for example MIN ARGMIN EXP LOG LEN symbols = == < >= precedingEndOfFile";

	std::cout << "\n" << "MANUAL TEST:" << std::endl;

	std::regex reg(R"(^[_a-z]\w*)");
	std::smatch match;
	if (std::regex_search(program, match, reg)) {
		std::cout << "match str " << match.str() << std::endl;
	}
	else {
		std::cout << "No match in " << program;
	}

	std::cout << "\n" << "LEXER TEST:" << std::endl;

	Lexer lexer = Lexer(program);

	//lexer.reset();

	const int maxIters = 50;

	for (int i = 0; i < maxIters; ++i) {
		try {
			Token newToken = lexer.nextToken();
			std::cout << "Iter: " << i << " New token: " << newToken.toString() << std::endl;
		}
		catch (const std::runtime_error& e) {
			std::cout << "Error in Iter: " << i << std::endl;
			std::cout << e.what() << std::endl;
		}	
	}

	std::cout << "RESET" << std::endl;

	lexer.reset();

	for (int i = 0; i < maxIters; ++i) {
		try {
			Token newToken = lexer.nextToken();
			std::cout << "Iter: " << i << " New token: " << newToken.toString() << std::endl;
		}
		catch (const std::runtime_error& e) {
			std::cout << "Error in Iter: " << i << std::endl;
			std::cout << e.what() << std::endl;
		}
	}


	return 0;
}
