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
		case Type::call_:
			return "call_";
		case Type::append_:
			return "append_";
		case Type::index_:
			return "index_";
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

	Token::Type Tokens::getType(const std::string& symbol) {
		auto lookUp = Tokens::TYPES.find(symbol);
		if (lookUp == Tokens::TYPES.end()) {
			return Token::Type::undefined;
		}
		else {
			return lookUp->second;
		}
	}

	Token Tokens::makeCall(const int& num_args) {
		Token call = Token(Token::Type::call_, CALL_);
		call.context.num_args = num_args;
		call.context.precedence = 10;
		return call;
	}

	Token Tokens::makeAppend(const int& num_args) {
		Token call = Token(Token::Type::append_, APPEND_);
		call.context.num_args = num_args;
		call.context.precedence = 10;
		return call;
	}

	Token Tokens::makeIndex(const int& num_args) {
		Token call = Token(Token::Type::index_, INDEX_);
		call.context.num_args = num_args;
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
					context.left_associative = false;
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				// leave precedence gap to above for unary +/-
				if ((tokenValue == MUL) || (tokenValue == DIV)) {
					context.precedence = 7;
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				if ((tokenValue == PLUS) || (tokenValue == MINUS)) {
					context.precedence = 6;
					context.maybe_infix = true;
					context.maybe_prefix = true;
					return Token(t, tokenValue, context);
				}
				if ((tokenValue == LT) ||
					(tokenValue == GT) ||
					(tokenValue == LE) ||
					(tokenValue == GE) ||
					(tokenValue == SMOOTH_LT) ||
					(tokenValue == SMOOTH_GT)) {
					context.precedence = 5;
					context.left_associative = false;
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == NOT) {
					context.precedence = 4;
					context.left_associative = false;
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == AND) {
					context.precedence = 3;
					context.left_associative = false;
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == OR) {
					context.precedence = 2;
					context.left_associative = false;
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				if (tokenValue == COMMA) {
					context.maybe_infix = true;
					context.is_infix = true;
					return Token(t, tokenValue, context);
				}
				return Token(type::undefined, tokenValue);
			}
			if (t == type::lparen) {
				context.precedence = 0;
				context.maybe_infix = true;
				context.maybe_prefix = true;
				return Token(t, tokenValue, context);
			}
			if (t == type::lbracket) {
				context.precedence = 0;
				context.maybe_prefix = true;
				return Token(t, tokenValue, context);
			}
		}
		if (t == type::tab) { return Token(t, "\t"); }  // review this
		if (t == type::eof) { return Token(t); }
		if ((t == type::rparen) || (t == type::rbracket)) { return Token(t, tokenValue); }
		return Token(type::undefined, tokenValue);
	}
}