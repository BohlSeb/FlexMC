// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <regex>
#include <unordered_map>
#include <stdexcept>

#include "tokens.h"
#include "lexer.h"
#include "terminals.h"

using namespace flexMC;

int main() {

	std::string inputNum = "3.34";

	Token token = Token(Token::Type::num, inputNum);

	Token sameToken = Token(Token::Type::num, inputNum);

	token.context.precedence += 1;

	ParsingContext newContext = ParsingContext(4);

	Token op = Token(Token::Type::num, inputNum, newContext);

	// Token plus = Tokens::makeOperator("+");

	short int numArgs = 2;

	Token call = Tokens::makeCall(numArgs);

	std::cout << "token: " << token << ", des: " << token.toString() << " prec " << token.context.precedence << std::endl;

	std::cout << "sameToken: " << sameToken << ", des: " << sameToken.toString() << " prec " << sameToken.context.precedence << std::endl;

	std::cout << "op: " << op << ", des: " << op.toString() << " prec " << op.context.precedence << std::endl;

	// std::cout << "plus: " << plus << ", des: " << plus.toString() << " prec " << plus.context.precedence << std::endl;

	std::cout << "call: " << call << ", des: " << call.toString() << " prec " << call.context.precedence << std::endl;

	return 0;
}
