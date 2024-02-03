#include <iostream>
#include <stdexcept>

#include "tokens.h"


namespace flexMC {

	// this is stupid but apparently not straight forward
	// https://stackoverflow.com/questions/3342726/c-print-out-enum-value-as-text
	// TODO: Remove this entirely (tokens do not care about string output)
	std::string Token::type2String() const {
		switch (type) {
		case Type::eof:
			return "eof";
		case Type::wsp:
			return "wsp";
		case Type::tab:
			return "tab";
		case Type::fun:
			return "fun";
		case Type::op:
			return "op";
		case Type::num:
			return "num";
		case Type::keyW:
			return "keyW";
		case Type::id:
			return "id";
		case Type::lparen:
			return "lparen";
		case Type::rparen:
			return "rparen";
		case Type::lbracket:
			return "lbracket";
		case Type::rbracket:
			return "rbracket";
		case Type::undefined:
			return "undefined";
		default:
			return "undefinedType";
		}
	}

	std::string Token::toString() const {
		std::string out;
		if ((type == Type::wsp) ||
			(type == Type::tab) ||
			(type == Type::eof)) {
			out = type2String();
		}
		else {
			out = value;
		}
		return "Tok(t=" + type2String() + ", v=" + out + ")";
	}


	std::ostream& operator<<(std::ostream& output, const Token& token) {
		if (token.type == Token::Type::op && ((token.value == CALL_) || (token.value == APPEND_))) {
			output << token.value << "(" << token.context.numArgs << ")";
		}
		else {
			output << token.value;
		}
		return output;
	}

	Token::Type Tokens::getType(const std::string& symbol) {
		auto lookUp = Tokens::TYPES.find(symbol);
		if (lookUp == Tokens::TYPES.end()) {
			return Token::Type::undefined;
		}
		else {
			return lookUp->second;
		}
	}

	Token Tokens::makeCall(unsigned int numArgs) {
		Token call = Token(Token::Type::op, CALL_);
		call.context.numArgs = numArgs;
		call.context.precedence = 10;
		return call;
	}

	Token Tokens::makeAppend(unsigned int numArgs) {
		Token call = Token(Token::Type::op, APPEND_);
		call.context.numArgs = numArgs;
		call.context.precedence = 10;
		return call;
	}

	Token Tokens::makeIndex(unsigned int numArgs) {
		Token call = Token(Token::Type::op, INDEX_);
		call.context.numArgs = numArgs;
		call.context.precedence = 10;
		return call;
	}

	// currently using python operator precedence
	// https://docs.python.org/3/reference/expressions.html

	Token Tokens::makeContextualized(const std::string& tokenValue) {
		using type = Token::Type;
		type t = getType(tokenValue);
		if (t == type::id || t == type::keyW || t == type::fun || t == type::num) {
			return Token(t, tokenValue);
		}
		if (t == type::wsp) { return Token(t, " "); }
		if ((t == type::op) || (t == type::lparen) || (t == type::lbracket)) {
			ParsingContext context = ParsingContext();
			if (t == type::op) {
				if (tokenValue == POW) {
					context.precedence = 9;
					context.leftAssociative = false;
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				// leave precedence gap to above for unary +/-
				if ((tokenValue == MUL) || (tokenValue == DIV)) {
					context.precedence = 7;
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				if ((tokenValue == PLUS) || (tokenValue == MINUS)) {
					context.precedence = 6;
					context.maybeInfix = true;
					context.maybePrefix = true;
					return Token(t, tokenValue, context);
				}
				if ((tokenValue == LT) ||
					(tokenValue == GT) ||
					(tokenValue == LE) ||
					(tokenValue == GE) ||
					(tokenValue == SMOOTH_LT) ||
					(tokenValue == SMOOTH_GT)) {
					context.precedence = 5;
					context.leftAssociative = false;
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == NOT) {
					context.precedence = 4;
					context.leftAssociative = false;
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == AND) {
					context.precedence = 3;
					context.leftAssociative = false;
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == OR) {
					context.precedence = 2;
					context.leftAssociative = false;
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == COMMA) {
					context.maybeInfix = true;
					context.isInfix = true;
					return Token(t, tokenValue, context);
				}
				return Token(type::undefined, tokenValue);
			}
			if (t == type::lparen) {
				context.precedence = 0;
				context.maybeInfix = true;
				context.maybePrefix = true;
				return Token(t, tokenValue, context);
			}
			if (t == type::lbracket) {
				context.precedence = 0;
				context.maybePrefix = true;
				return Token(t, tokenValue, context);
			}
		}
		if (t == type::tab) { return Token(t, "\t"); }  // review this
		if (t == type::eof) { return Token(t); }
		if ((t == type::rparen) || (t == type::rbracket)) { return Token(t, tokenValue); }
		return Token(type::undefined, tokenValue);
	}
}