#include <stdexcept>
#include <cassert>
#include <format>

#include "tokens.h"
#include "expression_parser.h"

// We try to implement this exceptional answer to a question about the shunting yard algorithm on stackoverflow:
// https://stackoverflow.com/questions/16380234/handling-extra-operators-in-shunting-yard/16392115#16392115
// https://en.wikipedia.org/wiki/Shunting_yard_algorithm


namespace flexMC {

	std::deque<Token> ExpressionParser::parseLine() {
		wantOperand();
		return output_;
	}

	void ExpressionParser::wantOperand() {
		using type = Token::Type;
		auto token = getNext();
		type t = token.type;

		// easy: is operand
		if (t == type::num || t == type::id || t == type::fun) {
			output_.push_front(token);
			haveOperand();
		}

		// prefix operator
		else if ((t == type::op && token.context.maybe_prefix)
			|| (t == type::lparen) || (t == type::lbracket)) {
			assert(!token.context.is_infix);
			token.context.is_prefix = true;
			token.context.is_infix = false;
			// jump mul/div
			if ((token.value == PLUS) || (token.value == MINUS)) {
				token.context.precedence += 2;
			}
			operators_.push_back(token);
			wantOperand();
		}

		// Unmatched parenthesis or "()" expected (function with 0 args)
		else if ((t == type::rparen) || (t == type::rbracket)) {
			if (operators_.empty()) {
				auto msg = std::format("Unmatched parenthesis/bracket: \"{}\"", token.value);
				exprLineParseError(msg);
			}
			if (operators_.back().context.num_args > 0) {
				exprLineParseError("Badly placed comma encountered within parentheses/brackets");
			}
			if (operators_.back().type == type::lbracket) {
				exprLineParseError("Empty list encountered: \"[]\"");
			}
			if (operators_.back().type != type::lparen) {
				auto msg = std::format("Expected empty argument list  \"()\", got \"{}\"", token.value);
				exprLineParseError(msg);
			}
			operators_.pop_back();
			operators_.push_back(Tokens::makeCall(0));
			haveOperand();
		}

		// Unexpected token
		else {
			auto msg = std::format(
				"Expected a variable, value, function name or a prefix operator, got \"{}\"", 
				token.value
			);
			if (t == type::eof) {
				msg += " (end of line)";
			}
			exprLineParseError(msg);
		}
	}

	void ExpressionParser::haveOperand() {
		using type = Token::Type;
		Token token = getNext();

		// base case / leave 2-state machine
		if (token.type == type::eof) {
			while (!operators_.empty()) {
				Token op = operators_.back();
				if ((op.type == type::lparen) || (op.type == type::lbracket)) {
					auto msg = std::format("Unmatched parenthesis/bracket: \"{}\"", token.value);
					exprLineParseError(msg);
				}
				output_.push_front(op);
				operators_.pop_back();
			}
			return;
		}

		// wiki page shunting yard
		if ((token.type == type::lparen) || (token.type == type::lbracket)) {
			assert(!token.context.is_prefix);
			token.context.is_infix = true;
			operators_.push_back(token);
			wantOperand();
		}

		// number of call args
		else if (token.value == COMMA) {
			std::string msg = "Unmatched parenthesis/bracket: \")\" or \"]\"";
			msg += " or badly placed comma \",\"";
			if (operators_.empty()) {
				exprLineParseError(msg);
			}
			type opType = operators_.back().type;
			while ((opType != type::lparen) && (opType != type::lbracket)) {
				output_.push_front(operators_.back());
				operators_.pop_back();
				if (operators_.empty()) {
					exprLineParseError(msg);
				}
				opType = operators_.back().type;
			}
			operators_.back().context.num_args += 1;
			wantOperand();
		}

		else if ((token.type == type::rparen) || (token.type == type::rbracket)) {
			auto msg = std::format("Unmatched parenthesis/bracket: \"{}\"", token.value);
			if (operators_.empty()) {
				exprLineParseError(msg);
			}
			type opType = operators_.back().type;
			type stop = (token.type == type::rparen) ? type::lparen : type::lbracket;
			while (opType != stop) {
				output_.push_front(operators_.back());
				operators_.pop_back();
				if (operators_.empty()) {
					exprLineParseError(msg);
				}
				opType = operators_.back().type;
			}
			Token leftClose = operators_.back();
			unsigned int num_args = leftClose.context.num_args + 1;
			if ((leftClose.type == type::lparen && leftClose.context.is_infix)) {
				output_.push_front(Tokens::makeCall(num_args));
			}
			else if (leftClose.type == type::lbracket && leftClose.context.is_prefix) {
				output_.push_front(Tokens::makeAppend(num_args));
			}
			else if (leftClose.type == type::lbracket && leftClose.context.is_infix) {
				output_.push_front(Tokens::makeIndex(num_args));
			}
			operators_.pop_back();
			haveOperand();
		}

		else if ((token.type == type::op) && (token.context.maybe_infix)) {
			while (!operators_.empty()) {
				ParsingContext opC = operators_.back().context;
				ParsingContext inputC = token.context;
				bool higher = opC.precedence > inputC.precedence;
				bool barelyHigher = (opC.precedence == inputC.precedence) && inputC.left_associative;
				if (higher || barelyHigher) {
					output_.push_front(operators_.back());
					operators_.pop_back();
				}
				else {
					break;
				}
			}
			assert(!token.context.is_prefix);
			token.context.is_infix = true;
			operators_.push_back(token);
			wantOperand();

		}

		// Unexpected token
		else {
			auto msg = std::format(
				"Expected an operator, got \"{0}\" of type {1}", 
				token.value, 
				token.type2String()
			);
			if (token.type == type::eof) {
				msg += " (end of line)";
			}
			exprLineParseError(msg);
		}
	}

	Token ExpressionParser::getNext() {
		auto token = lexer_.nextToken();
		auto type = token.type;
		if (type == Token::Type::undefined) {
			auto msg = std::format("No valid language token found at the beginning of \"{}\"", token.value);
			exprLineParseError(msg);
		}
		if ((type == Token::Type::wsp) || (type == Token::Type::tab)) {
			parsed << token.value;
			return getNext();
		}
		parsed << token.value;
		return token;
	}

	void ExpressionParser::exprLineParseError(const std::string& message) const {
		auto msg = std::format("Parsed >> \"{0}\" \nProblem >> {1}", parsed.str(), message);
		throw std::runtime_error(msg);
	}

}
