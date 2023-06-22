#pragma once

#include <iostream>
#include <unordered_map>

#include "terminals.h"



namespace flexMC {

	struct ParsingContext {
		unsigned short int precedence;
		bool maybePrefix = false;
		bool maybeInfix = false;
		bool isPrefix = false;
		bool isInfix = false;
		short int numArgs = 0;
		bool leftAssociative = true;

		ParsingContext() : precedence(1) {}

		ParsingContext(unsigned short int precedence) : precedence(precedence) {}

		ParsingContext(unsigned short int precedence, 
			bool maybePrefix, 
			bool maybeInfix, 
			bool isPrefix, 
			bool isInfix) : 
			precedence(precedence),
			maybePrefix(maybePrefix),
			maybeInfix(maybeInfix),
			isPrefix(isPrefix),
			isInfix(isInfix) {}
	};

	struct Token {

		enum class Type {
			eof,
			eol,
			wsp,
			tab,
			op,
			fun,
			num,
			keyW,
			id,
			undefined
		};

		Token(Type type,const std::string value = "") : 
			type(type),
			value(value),
			context(ParsingContext()) {}

		Token(Type type, const std::string value, ParsingContext context) : 
			type(type),
			value(value),
			context(context) {}

		std::string toString() const;
		std::string type2String() const;

		const Type type;
		const std::string value;
		ParsingContext context;
	};

	std::ostream& operator<<(std::ostream& output, const Token& token);

	namespace Tokens {

		Token::Type getType(const std::string& symbol);

		Token makeContextualized(const std::string& symbol);

		Token makeCall(short int& numArgs);

		Token makeAppend(short int& numArgs);

		const std::unordered_map<std::string, Token::Type> TYPES = {

			{IF, Token::Type::keyW},
			{THEN, Token::Type::keyW},
			{ELSE, Token::Type::keyW},
			{PAY, Token::Type::keyW},
			{STOP, Token::Type::keyW},
			{ASSIGN, Token::Type::keyW},
			{PLUS_ASSIGN, Token::Type::keyW},
			{MINUS_ASSIGN, Token::Type::keyW},
			{MUL_ASSIGN, Token::Type::keyW},
			{DIV_ASSIGN, Token::Type::keyW},
			{POW_ASSIGN, Token::Type::keyW},

			{EXP, Token::Type::fun},
			{LOG, Token::Type::fun},
			{ABS, Token::Type::fun},
			{MIN, Token::Type::fun},
			{MAX, Token::Type::fun},
			{SUM, Token::Type::fun},
			{A_MEAN, Token::Type::fun},
			{G_MEAN, Token::Type::fun},
			{ARGMIN, Token::Type::fun},
			{ARGMAX, Token::Type::fun},
			{LEN, Token::Type::fun},

			{EQ, Token::Type::op},
			{PLUS, Token::Type::op},
			{MINUS, Token::Type::op},
			{MUL, Token::Type::op},
			{DIV, Token::Type::op},
			{POW, Token::Type::op},
			{LT, Token::Type::op},
			{GT, Token::Type::op},
			{LE, Token::Type::op},
			{GE, Token::Type::op},
			{NOT, Token::Type::op},
			{AND, Token::Type::op},
			{OR, Token::Type::op},
			{SMOOTH_LT, Token::Type::op},
			{SMOOTH_GT, Token::Type::op},
			{COMMA, Token::Type::op},
			{P_LEFT, Token::Type::op},
			{P_RIGHT, Token::Type::op},
			{B_LEFT, Token::Type::op},
			{B_RIGHT, Token::Type::op},

			{"\n", Token::Type::eol},
			{"\t", Token::Type::tab},
			{" ", Token::Type::wsp},
		};
	}
}
