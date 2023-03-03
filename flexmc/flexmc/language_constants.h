#pragma once

#include <iostream>
#include <unordered_map>

#include "lexer.h"

namespace flexMC {

	const std::string IF = "IF";
	const std::string THEN = "THEN";
	const std::string ELSE = "ELSE";
	const std::string AND = "AND";
	const std::string OR = "OR";
	const std::string PAY = "PAY";
	const std::string STOP = "STOP";
	const std::string EXP = "EXP";
	const std::string LOG = "LOG";
	const std::string ABS = "ABS";
	const std::string MIN = "MIN";
	const std::string MAX = "MAX";
	const std::string SUM = "SUM";
	const std::string A_MEAN = "AMEAN";
	const std::string G_MEAN = "GMEAN";
	const std::string ARGMIN = "ARGMIN";
	const std::string ARGMAX = "ARGMAX";
	const std::string LEN = "LEN";
	const std::string APPEND = "APPEND";

	const std::string ASSIGN = "=";
	const std::string EQ = "==";
	const std::string PLUS = "+";
	const std::string MINUS = "-";
	const std::string MUL = "*";
	const std::string DIV = "/";
	const std::string POW = "^";
	const std::string LT = "<";
	const std::string GT = ">";
	const std::string LE = "<=";
	const std::string GE = ">=";
	const std::string SMOOTH_LT = "<<";
	const std::string SMMOTH_GT = ">>";
	const std::string COMMA = ",";
	const std::string P_LEFT = "(";
	const std::string P_RIGHT = ")";
	const std::string B_LEFT = "[";
	const std::string B_RIGHT = "]";

	const std::unordered_map<std::string, Token::Type> TYPES = {

		{IF, Token::Type::keyWord},
		{THEN, Token::Type::keyWord},
		{ELSE, Token::Type::keyWord},
		{AND, Token::Type::keyWord},
		{OR, Token::Type::keyWord},
		{PAY, Token::Type::keyWord},
		{STOP, Token::Type::keyWord},
		{EXP, Token::Type::keyWord},
		{LOG, Token::Type::keyWord},
		{ABS, Token::Type::keyWord},
		{MIN, Token::Type::keyWord},
		{MAX, Token::Type::keyWord},
		{SUM, Token::Type::keyWord},
		{A_MEAN, Token::Type::keyWord},
		{G_MEAN, Token::Type::keyWord},
		{ARGMIN, Token::Type::keyWord},
		{ARGMAX, Token::Type::keyWord},
		{LEN, Token::Type::keyWord},
		{APPEND, Token::Type::keyWord},

		{ASSIGN, Token::Type::sym},
		{EQ, Token::Type::sym},
		{PLUS, Token::Type::sym},
		{MINUS, Token::Type::sym},
		{MUL, Token::Type::sym},
		{DIV, Token::Type::sym},
		{POW, Token::Type::sym},
		{LT, Token::Type::sym},
		{GT, Token::Type::sym},
		{LE, Token::Type::sym},
		{GE, Token::Type::sym},
		{SMOOTH_LT, Token::Type::sym},
		{SMMOTH_GT, Token::Type::sym},
		{COMMA, Token::Type::sym},
		{P_LEFT, Token::Type::sym},
		{P_RIGHT, Token::Type::sym},
		{B_LEFT, Token::Type::sym},
		{B_RIGHT, Token::Type::sym},

		{"\n", Token::Type::eol},
		{"\t", Token::Type::tab},
		{" ", Token::Type::wsp}

	};

	// Identifiers must start with either a lower case letter or "_"

	const std::string R_ID = R"(^[_a-z]\w*)";
	const std::string R_NUM = R"(^(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?)";

	const std::string R_GROUPS[] = {
		R"(^IF)",
		R"(^THEN)",
		R"(^ELSE)",
		R"(^AND)",
		R"(^OR)",
		R"(^PAY)",
		R"(^STOP)",
		R"(^EXP)",
		R"(^LOG)",
		R"(^ABS)",
		R"(^MIN)",
		R"(^MAX)",
		R"(^SUM)",
		R"(^AMEAN)",
		R"(^GMEAN)",
		R"(^ARGMIN)",
		R"(^ARGMAX)",
		R"(^LEN)",
		R"(^APPEND)",
		R"(^=(=?))",
		R"(^\+(=?))",
		R"(^\-(=?))",
		R"(^\*(=?))",
		R"(^\/(=?))",
		R"(^\^(=?))",
		R"(^\<(=?))",
		R"(^\>(=?))",
		R"(^\<(\<?))",
		R"(^\>(\>?))",
		R"(^\,)",
		R"(^\()",
		R"(^\))",
		R"(^\[)",
		R"(^\])",
		R"(^\n)",
		R"(^\t)",
		R"(^\s)",
	};
};