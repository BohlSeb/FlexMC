#include <stdexcept>
#include <cassert>
#include <iomanip>
#include <sstream>

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
		else if ((t == type::op && token.context.maybePrefix)
			|| (t == type::lparen) || (t == type::lbracket)) {
			assert(!token.context.isInfix);
			token.context.isPrefix = true;
			token.context.isInfix = false;
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
				exprLineParseError("Unmatched paranthesis: \")\"");
			}
			if (operators_.back().type == type::lbracket) {
				exprLineParseError("\"[]\" - empty list or index not supported");
			}
			if (operators_.back().type != type::lparen) {
				std::stringstream msg;
				msg << "Expected: " << std::quoted("()");
				msg << ", got: " << std::quoted(token.value);
				exprLineParseError(msg.str());
			}
			operators_.pop_back();
			operators_.emplace_back(Tokens::makeCall(0));
			haveOperand();
		}
		// Unexpted token
		else {
			std::stringstream msg;
			msg << "Expected an operand (Variable/Value/Function name) or an infix operator";
			msg << " (" << std::quoted("+, -, *, /, ',' etc") << ")";
			msg << ", got: " << std::quoted(token.value) << "; type: " << token.type2String();
			if (t == type::eof) {
				msg << " (end of line)";
			}
			exprLineParseError(msg.str());
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

					std::stringstream msg;
					msg << "Unmatched parenthesis/bracket: ";
					msg << std::quoted(op.value);
					exprLineParseError(msg.str());
				}
				output_.push_front(op);
				operators_.pop_back();
			}
			return;
		}

		// wiki page shunting yard
		if ((token.type == type::lparen) || (token.type == type::lbracket)) {
			assert(!token.context.isPrefix);
			token.context.isInfix = true;
			operators_.push_back(token);
			wantOperand();
		}

		// number of call args
		else if (token.value == COMMA) {
			std::stringstream msg;
			msg << "Unmatched parenthesis/bracket: \")\" or \"]\"";
			msg << " or badly placed comma \",\"";
			if (operators_.empty()) {
				exprLineParseError(msg.str());
			}
			type opType = operators_.back().type;
			while ((opType != type::lparen) && (opType != type::lbracket)) {
				output_.push_front(operators_.back());
				operators_.pop_back();
				if (operators_.empty()) {
					exprLineParseError(msg.str());
				}
				opType = operators_.back().type;
			}
			Token leftClose = operators_.back();

			// exclude standard left parenthesis (not a function call)
			if (!(leftClose.type == type::lparen && leftClose.context.isPrefix)) {
				leftClose.context.numArgs += 1;
			}
			wantOperand();
		}

		else if ((token.type == type::rparen) || (token.type == type::rbracket)) {
			std::stringstream msg;
			msg << "Unmatched parenthesis/bracket: ";
			msg << std::quoted(token.value);
			if (operators_.empty()) {
				exprLineParseError(msg.str());
			}
			type opType = operators_.back().type;
			type stop = (token.type == type::rparen) ? type::lparen : type::lbracket;
			while (opType != stop) {
				output_.push_front(operators_.back());
				operators_.pop_back();
				if (operators_.empty()) {
					exprLineParseError(msg.str());
				}
				opType = operators_.back().type;
			}
			Token leftClose = operators_.back();
			unsigned int numArgs = leftClose.context.numArgs + 1;
			if ((leftClose.type == type::lparen && leftClose.context.isInfix)) {
				output_.emplace_front(Tokens::makeCall(numArgs));
			}
			else if (leftClose.type == type::lbracket && leftClose.context.isPrefix) {
				output_.emplace_front(Tokens::makeAppend(numArgs));
			}
			else if (leftClose.type == type::lbracket && leftClose.context.isInfix) {
				output_.emplace_front(Tokens::makeIndex(numArgs));
			}
			operators_.pop_back();
			haveOperand();
		}

		else if ((token.type == type::op) && (token.context.maybeInfix)) {
			// while ((!operators_.empty()) && (operators_.back().type != type::lparen))
			while (!operators_.empty()) {
				ParsingContext opC = operators_.back().context;
				ParsingContext inputC = token.context;
				bool higher = opC.precedence > inputC.precedence;
				bool barelyHigher = (opC.precedence == inputC.precedence) && inputC.leftAssociative;
				if (higher || barelyHigher) {
					output_.push_front(operators_.back());
					operators_.pop_back();
				}
				else {
					break;
				}
			}
			assert(!token.context.isPrefix);
			token.context.isInfix = true;
			operators_.push_back(token);
			wantOperand();

		}

		// Unexpted token
		else {
			std::stringstream msg;
			msg << "Expected an operator";
			msg << ", got: " << std::quoted(token.value) << "; type: " << token.type2String();
			if (token.type == type::eof) {
				msg << " (end of line)";
			}
			exprLineParseError(msg.str());
		}
	}

	Token ExpressionParser::getNext() {
		auto token = lexer_.nextToken();
		auto type = token.type;
		if (type == Token::Type::undefined) {
			exprLineParseError("Token type undefinend / unrecognizable word (maybe regex problem)");
		}
		if ((type == Token::Type::wsp) || (type == Token::Type::tab)) {
			parsed << token.value;
			return getNext();
		}
		parsed << token.value;
		return token;
	}

	void ExpressionParser::exprLineParseError(const std::string& message) {
		std::stringstream msg;
		msg << "Parsed: ";
		msg << std::quoted(parsed.str());
		msg << ", Problem: ";
		msg << message;
		throw std::runtime_error(msg.str());
	}

}
