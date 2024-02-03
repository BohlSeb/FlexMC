#pragma once

#include <deque>
#include <vector>
#include <sstream>

#include "lexer.h"
#include "tokens.h"

namespace flexMC {

	class ExpressionParser {

	public:

		ExpressionParser(Lexer& lexer) : lexer_(lexer), numOperands_(0), parsed("") {}

		std::deque<Token> parseLine();

		inline const int numOperands() { return numOperands_; }

		std::stringstream parsed;

	private:

		Lexer& lexer_;
		std::deque<Token> output_;
		std::vector<Token> operators_;
		unsigned int numOperands_;

		void wantOperand();
		void haveOperand();
		// todo: custom exceptions?
		void exprLineParseError(const std::string& message);
		Token getNext(); // private

	};

}
