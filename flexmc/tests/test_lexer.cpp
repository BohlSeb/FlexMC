#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <vector>

#include "../flexmc/lexer.h"
#include "../flexmc/lexer.cpp"
#include "../flexmc/tokens.cpp"


using namespace flexMC;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



TEST_CLASS(TestLexer) {
public:

	TEST_METHOD(TestValidKeywordsAndSymbols) {

		Token::Type kW = Token::Type::keyW;
		Token::Type op = Token::Type::op;
		Token::Type fun = Token::Type::fun;

		std::vector<Token> expected = {
			Token(kW, "IF"),
			Token(kW, "THEN"),
			Token(kW, "ELSE"),
			Token(kW, "PAY"),
			Token(kW, "STOP"),
			Token(kW, ":="),
			Token(kW, "+="),
			Token(kW, "-="),
			Token(kW, "*="),
			Token(kW, "/="),
			Token(kW, "^="),
			Token(fun, "EXP"),
			Token(fun, "LOG"),
			Token(fun, "ABS"),
			Token(fun, "MIN"),
			Token(fun, "MAX"),
			Token(fun, "SUM"),
			Token(fun, "AMEAN"),
			Token(fun, "GMEAN"),
			Token(fun, "ARGMIN"),
			Token(fun, "ARGMAX"),
			Token(fun, "LEN"),
			Token(op, "AND"),
			Token(op, "OR"),
			Token(op, "NOT"),
			Token(op, "=="),
			Token(op, "+"),
			Token(op, "-"),
			Token(op, "*"),
			Token(op, "/"),
			Token(op, "^"),
			Token(op, "<<"),
			Token(op, ">>"),
			Token(op, "<"),
			Token(op, ">"),
			Token(op, ","),
			Token(op, "("),
			Token(op, ")"),
			Token(op, "<="),
			Token(op, ">="),
			Token(op, "["),
			Token(op, "]"),
		};

		std::string testStr = "IF";
		testStr += "THEN";
		testStr += "ELSE";
		testStr += "PAY";
		testStr += "STOP";
		testStr += ":=";
		testStr += "+=";
		testStr += "-=";
		testStr += "*=";
		testStr += "/=";
		testStr += "^=";
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
		testStr += "AND";
		testStr += "OR";
		testStr += "NOT";
		testStr += "==";
		testStr += "+";
		testStr += "-";
		testStr += "*";
		testStr += "/";
		testStr += "^";
		testStr += "<<";
		testStr += ">>";
		testStr += "<";
		testStr += ">";
		testStr += ",";
		testStr += "(";
		testStr += ")";
		testStr += "<=";
		testStr += ">=";
		testStr += "[";
		testStr += "]";


		Lexer lexer = Lexer(testStr);
		int i = 0;
		for (auto it = std::begin(expected); it != std::end(expected); ++it) {
			Token token = lexer.nextToken();
			Token expected = *it;
			Assert::AreEqual(expected.toString(), token.toString());
		}
		Token end = lexer.nextToken();
		Token endExpected = Token(Token::Type::eof);
		Assert::AreEqual(endExpected.toString(), end.toString());
	}

	TEST_METHOD(TestValidNumbersAndIdentifiers) {

		Token::Type num = Token::Type::num;
		Token::Type id = Token::Type::id;
		Token::Type wsp = Token::Type::wsp;
		Token::Type op = Token::Type::op;
		Token::Type tab = Token::Type::tab;

		std::vector<Token> expected = {
			Token(id, "variable"),
			Token(wsp),
			Token(id, "_variable"),
			Token(wsp),
			Token(id, "_USER_CONSTANT"),
			Token(wsp),
			Token(id, "userNumber10"),
			Token(wsp),
			Token(id, "userVariable"),
			Token(wsp),
			Token(id, "min"),
			Token(wsp),
			Token(id, "userValue10Suffix"),
			Token(wsp),
			Token(id, "userValue_10_Suffix"),
			Token(wsp),
			Token(id, "_USER_CONSTANT_101"),
			Token(wsp),
			Token(id, "_USER_110_CONSTANT"),
			Token(wsp),
			Token(num, "0"),
			Token(wsp),
			Token(num, "0.05"),
			Token(wsp),
			Token(num, "100"),
			Token(wsp),
			Token(num, "921039112"),
			Token(wsp),
			Token(num, "100.3"),
			Token(wsp),
			Token(num, "1e3"),
			Token(wsp),
			Token(num, "1.02E3"),
			Token(wsp),
			Token(num, "1.02E-3"),
			Token(wsp),
			Token(num, "4e-5"),
			Token(wsp),
			Token(num, "1."),
			Token(wsp),
			Token(num, ".555"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "0"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "0.0000"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "0.005"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "100.3"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "1e3"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "1.02E3"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "921039112"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "4e-5"),
			Token(wsp),
			Token(op, "-"),
			Token(num, "1."),
			Token(wsp),
			Token(op, "-"),
			Token(num, ".555"),
			Token(wsp),
			Token(op, "-"),
			Token(wsp),
			Token(num, ".555"),
			Token(wsp),
			Token(op, "-"),
			Token(wsp),
			Token(num, "1."),
			Token(tab),
			Token(op, "-"),
			Token(tab),
			Token(num, ".555"),
			Token(tab),
			Token(op, "-"),
			Token(tab),
			Token(num, "1."),
		};

		std::string testStr = "variable";
		testStr += " ";
		testStr += "_variable";
		testStr += " ";
		testStr += "_USER_CONSTANT";
		testStr += " ";
		testStr += "userNumber10";
		testStr += " ";
		testStr += "userVariable";
		testStr += " ";
		testStr += "min";
		testStr += " ";
		testStr += "userValue10Suffix";
		testStr += " ";
		testStr += "userValue_10_Suffix";
		testStr += " ";
		testStr += "_USER_CONSTANT_101";
		testStr += " ";
		testStr += "_USER_110_CONSTANT";
		testStr += " ";
		testStr += "0";
		testStr += " ";
		testStr += "0.05";
		testStr += " ";
		testStr += "100";
		testStr += " ";
		testStr += "921039112";
		testStr += " ";
		testStr += "100.3";
		testStr += " ";
		testStr += "1e3";
		testStr += " ";
		testStr += "1.02E3";
		testStr += " ";
		testStr += "1.02E-3";
		testStr += " ";
		testStr += "4e-5";
		testStr += " ";
		testStr += "1.";
		testStr += " ";
		testStr += ".555";
		testStr += " ";
		testStr += "-0";
		testStr += " ";
		testStr += "-0.0000";
		testStr += " ";
		testStr += "-0.005";
		testStr += " ";
		testStr += "-100.3";
		testStr += " ";
		testStr += "-1e3";
		testStr += " ";
		testStr += "-1.02E3";
		testStr += " ";
		testStr += "-921039112";
		testStr += " ";
		testStr += "-4e-5";
		testStr += " ";
		testStr += "-1.";
		testStr += " ";
		testStr += "-.555";
		testStr += " ";
		testStr += "- .555";
		testStr += " ";
		testStr += "- 1.";
		testStr += "\t";
		testStr += "-\t.555";
		testStr += "\t";
		testStr += "-\t1.";



		Lexer lexer = Lexer(testStr);

		for (auto it = std::begin(expected); it != std::end(expected); ++it) {
			Token token = lexer.nextToken();
			Token expected = *it;
			Assert::AreEqual(expected.toString(), token.toString());
		}
		Token end = lexer.nextToken();
		Token endExpected = Token(Token::Type::eof);
		Assert::AreEqual(endExpected.toString(), end.toString());
	}

};

