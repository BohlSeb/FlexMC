#include <iostream>

#include "tokens.h"


namespace flexMC {

	std::string Token::type2String() const {
		switch (type) {
		case Type::eof:
			return "eof";
		case Type::eol:
			return "eol";
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
		case Type::undefined:
			return "undefined";
		default:
			return "";
		}
	}

	std::string Token::toString() const {
		std::string out;
		if ((type == Type::wsp) || (type == Type::eol) || (type == Type::tab) || (type == Type::eof)) {
			out = type2String();
		}
		else {
			out = value;
		}
		return "Tok(t=" + type2String() + ", v=" + out + ")";
	}


	std::ostream& operator<<(std::ostream& output, const Token& token) {
		if (token.type == Token::Type::op && ((token.value == "call") || (token.value == "append"))) {
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

	Token Tokens::makeCall(short int& numArgs) {
		Token call = Token(Token::Type::op, CALL_);
		call.context.numArgs = numArgs;
		return call;
	}

	Token Tokens::makeAppend(short int& numArgs) {
		Token call = Token(Token::Type::op, APPEND_);
		call.context.numArgs = numArgs;
		return call;
	}

	Token Tokens::makeContextualized(const std::string& symbol) {
		Token::Type type = getType(symbol);
		switch (type) {
		case Token::Type::wsp: {
			return Token(type);
		}
		case Token::Type::eof: {
			return Token(type);
		}
		case Token::Type::eol: {
			return Token(type);
		}
		case Token::Type::tab: {
			return Token(type);
		}
		case Token::Type::keyW: {
			return Token(type, symbol);
		}
		case Token::Type::num: {
			return Token(type, symbol);
		}
		case Token::Type::id: {
			return Token(type, symbol);
		}
		case Token::Type::fun: {
			return Token(type, symbol);
		}
		// currently using python operator precedence https://docs.python.org/3/reference/expressions.html
		case Token::Type::op: {
			ParsingContext context = ParsingContext();
			if (symbol == POW) {
				context.precedence = 9;
				context.leftAssociative = false;
				context.maybeInfix = true;
				context.isInfix = true;
				return Token(type, symbol, context);
			}

			// leave gap to above for unary +/-
			if ((symbol == MUL) || (symbol == DIV)) {
				context.precedence = 7;
				context.maybeInfix = true;
				context.isInfix = true;
				return Token(type, symbol, context);
			}

			if ((symbol == PLUS) || (symbol == MINUS)) {
				context.precedence = 6;
				context.maybeInfix = true;
				context.maybePrefix = true;
				return Token(type, symbol, context);
			}

			if ((symbol == EQ) ||
				(symbol == LT) ||
				(symbol == GT) ||
				(symbol == LE) ||
				(symbol == GE) ||
				(symbol == SMOOTH_LT) ||
				(symbol == SMOOTH_GT)) {
				context.precedence = 5;
				context.leftAssociative = false;
				context.maybeInfix = true;
				context.isInfix = true;
				return Token(type, symbol, context);
			}

			if (symbol == NOT) {
				context.precedence = 4;
				context.leftAssociative = false;
				context.maybeInfix = true;
				context.isInfix = true;
				return Token(type, symbol, context);
			}

			if (symbol == AND) {
				context.precedence = 3;
				context.leftAssociative = false;
				context.maybeInfix = true;
				context.isInfix = true;
				return Token(type, symbol, context);
			}

			if (symbol == OR) {
				context.precedence = 2;
				context.leftAssociative = false;
				context.maybeInfix = true;
				context.isInfix = true;
				return Token(type, symbol, context);
			}

			if ((symbol == P_LEFT) || (symbol == B_LEFT)) {
				context.maybeInfix = true;
				context.maybePrefix = true;
				return Token(type, symbol);
			}

			if ((symbol == P_RIGHT) || (symbol == B_RIGHT) || (symbol == COMMA)) {
				return Token(type, symbol);
			}

			return Token(Token::Type::undefined, "operator");
		}
		case Token::Type::undefined: {
			return Token(type, symbol);
		}
		default: {return Token(Token::Type::undefined, "terminal"); }
		}
	}
}