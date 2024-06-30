#include <gtest/gtest.h>

#include <ranges>

#include "lexer.h"
#include "expression_parser.h"


TEST(ExpressionParser, OperatorsAndOperands) {

	using namespace flexMC;

	// using namespace flexMC;
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

    Lexer lexer;

	for (auto& c : TestData) {

        const std::deque<Token> infix = lexer.tokenize(c.infix);
		const std::pair<const MaybeError, const std::vector<Token>> parse_result = infixToPostfix(infix);
        const auto report = parse_result.first;
        EXPECT_FALSE(report.isError());
        const auto postfix = parse_result.second;
		std::string res;
		size_t i = 0;
		auto size = postfix.size();
		for (auto & it : postfix) {
			res += it.value;
			if (i < size - 1) {
				res += " ";
			};
			++i;
		}
		EXPECT_EQ(res, c.postfix);

	}

}