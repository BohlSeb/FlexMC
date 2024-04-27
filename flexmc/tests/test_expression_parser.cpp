#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "../flexmc/expression_parser.cpp"

using namespace flexMC;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



TEST_CLASS(TestExpressionParser) {
public:

	TEST_METHOD(TestBasicOperatorsAndOperands) {
		struct TestCase {
			std::string infix;
			std::string postfix;
		};

		// Test cases from chat gbt

		TestCase TestData[] = {
			{"2 + 3", "2 3 +"},
			{"a * b - c", "a b * c -"},
			{"a + b * c", "a b c * +"},
			{"3 * (5 + 2)", "3 5 2 + *"},
			{"(a + b) * c", "a b + c *"},
			{"a * (b + c)", "a b c + *"},
			{"2 * x + 3", "2 x * 3 +"},
			{"a * 5 - b", "a 5 * b -"},
			{"a + b * (c - d) / e", "a b c d - * e / +"},
			{"((a + b) * c - d) / e", "a b + c * d - e /"},
			{"a + b * c", "a b c * +"},
			{"a * b + c", "a b * c +"},
			{"a**2 + b ** 3", "a 2 ** b 3 ** +"},
			{"x**(y+1)", "x y 1 + **"},
			{"a + b ** 2  * c", "a b 2 ** c * +"},
			{"2 * x ** 3 - 5", "2 x 3 ** * 5 -"},
			{"(a + b)**2", "a b + 2 **"},
			{"x **(y + z) / 2", "x y z + ** 2 /"},
		};

		for (auto& c : TestData) {

			Lexer lexer = Lexer(c.infix);
			ExpressionParser parser = ExpressionParser(lexer);
			std::deque<Token> parsed = parser.parseLine();
			std::stringstream res;
			using riter = std::deque<Token>::reverse_iterator;
			int i = 0;
			auto size = parsed.size();
			for (riter it = parsed.rbegin(); it != parsed.rend(); ++it) {
				res << *it;
				if (i < size - 1) {
					res << " ";
				};
				++i;
			}
			Assert::AreEqual(res.str(), c.postfix);

		}

	}
};