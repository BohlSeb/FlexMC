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

	TEST_METHOD(TestValidKeywordsAndSymbols) {

		Token::Type kW = Token::Type::keyWord;
		Token::Type sym = Token::Type::sym;

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
			Token(sym, "=="),
			Token(sym, "="),
			Token(sym, "+"),
			Token(sym, "-"),
			Token(sym, "*"),
			Token(sym, "/"),
			Token(sym, "^"),
			Token(sym, "+="),
			Token(sym, "-="),
			Token(sym, "*="),
			Token(sym, "/="),
			Token(sym, "^="),
			Token(sym, "<<"),
			Token(sym, ">>"),
			Token(sym, "<"),
			Token(sym, ">"),
			Token(sym, ","),
			Token(sym, "("),
			Token(sym, ")"),
			Token(sym, "<="),
			Token(sym, ">="),
			Token(sym, "["),
			Token(sym, "]"),
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
		testStr += "==";
		testStr += "=";
		testStr += "+";
		testStr += "-";
		testStr += "*";
		testStr += "/";
		testStr += "^";
		testStr += "+=";
		testStr += "-=";
		testStr += "*=";
		testStr += "/=";
		testStr += "^=";
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

		for (auto it = std::begin(expected); it != std::end(expected); ++it) {
			Token token = lexer.nextToken();
			Token expected = *it;
			Assert::AreEqual(expected.type2String(), token.type2String());
			Assert::AreEqual(expected.value, token.value);
		}
		Token end = lexer.nextToken();
		Token endExpected = Token(Token::Type::eof);
		Assert::AreEqual(endExpected.type2String(), end.type2String());
		Assert::AreEqual(endExpected.value, end.value);
	}

	TEST_METHOD(TestValidNumbersAndIdentifiers) {

		Token::Type num = Token::Type::number;
		Token::Type id = Token::Type::id;
		Token::Type wsp = Token::Type::wsp;
		Token::Type sym = Token::Type::sym;
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
			Token(sym, "-"),
			Token(num, "0"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "0.0000"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "0.005"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "100.3"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "1e3"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "1.02E3"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "921039112"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "4e-5"),
			Token(wsp),
			Token(sym, "-"),
			Token(num, "1."),
			Token(wsp),
			Token(sym, "-"),
			Token(num, ".555"),
			Token(wsp),
			Token(sym, "-"),
			Token(wsp),
			Token(num, ".555"),
			Token(wsp),
			Token(sym, "-"),
			Token(wsp),
			Token(num, "1."),
			Token(tab),
			Token(sym, "-"),
			Token(tab),
			Token(num, ".555"),
			Token(tab),
			Token(sym, "-"),
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
			Assert::AreEqual(expected.type2String(), token.type2String());
			Assert::AreEqual(expected.value, token.value);
		}
		Token end = lexer.nextToken();
		Token endExpected = Token(Token::Type::eof);
		Assert::AreEqual(endExpected.type2String(), end.type2String());
		Assert::AreEqual(endExpected.value, end.value);
	}

};

