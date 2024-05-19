#pragma once

#include <deque>
#include <vector>
#include <sstream>

#include "lexer.h"
#include "tokens.h"

namespace flexMC {

	class ExpressionParser {

	public:

		explicit ExpressionParser(Lexer& lexer) : parsed(""), lexer_(lexer) {}

		std::deque<Token> parseLine();

		std::stringstream parsed;

	private:

        enum class Next {
            want_operand,
            have_operand,
            end
        };

		Lexer& lexer_;

		std::deque<Token> output_;

		std::vector<Token> operators_;

		Token getNext();

		void wantOperand();

		void haveOperand();

		// todo: custom exceptions?
		void exprLineParseError(const std::string& message) const;

	};

}
