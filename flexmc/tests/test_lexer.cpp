#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <vector>

#include "../flexmc/lexer.h"
#include "../flexmc/lexer.cpp"

using namespace flexMC;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



TEST_CLASS(TestLexer) {
public:

	TEST_METHOD(TestKewords) {

		Token::Type kW = Token::Type::keyWord;

		std::vector<Token> expected = {
			Token(kW, "IF"),
			Token(kW, "THEN"),
			Token(kW, "ELSE"),
			Token(kW, "AND"),
			Token(kW, "OR"),
			Token(kW, "PAY"),
			Token(kW, "STOP"),
			Token(kW, "EXP"),
			Token(kW, "LOG"),
			Token(kW, "ABS"),
			Token(kW, "MIN"),
			Token(kW, "MAX"),
			Token(kW, "SUM"),
			Token(kW, "AMEAN"),
			Token(kW, "GMEAN"),
			Token(kW, "ARGMIN"),
			Token(kW, "ARGMAX"),
			Token(kW, "LEN"),
			Token(kW, "APPEND"),
		};

		std::string testStr = "IF";
		testStr += "THEN";
		testStr += "ELSE";
		testStr += "AND";
		testStr += "OR";
		testStr += "PAY";
		testStr += "STOP";
		testStr += "EXP";
		testStr += "LOG";
		testStr += "ABS";
		testStr += "MIN";
		testStr += "MAX";
		testStr += "SUM";
		testStr += "AMEAN";
		testStr += "GMEAN";
		testStr += "ARGMIN";
		testStr += "ARGMAX";
		testStr += "LEN";
		testStr += "APPEND";

		Lexer lexer = Lexer(testStr);

		for (auto it = std::begin(expected); it != std::end(expected); ++it) {
			Token token = lexer.nextToken();
			Token expected = *it;
			Assert::AreEqual(token.type2String(), expected.type2String());
			Assert::AreEqual(token.value, expected.value);
		}
		Token end = lexer.nextToken();
		Token endExpected = Token(Token::Type::eof);
		Assert::AreEqual(end.type2String(), endExpected.type2String());
	}
};

