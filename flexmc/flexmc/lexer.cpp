#include <string>

#include "lexer.h"
#include "language_constants.h"

namespace flexMC {

	std::string Token::type2String() {
		switch (type) {
		case Type::eof:
			return "eof";
		case Type::eol:
			return "eol";
		case Type::wsp:
			return "wsp";
		case Type::tab:
			return "tab";
		case Type::sym:
			return "sym";
		case Type::number:
			return "number";
		case Type::keyWord:
			return "keyWord";
		case Type::id:
			return "id";
		default:
			return "";
		}
	}

	std::string Token::toString() {
		std::string res = "Token(type=" + type2String()
			+ ", value=" + value
			+ ", pos=" + std::to_string(position)
			+ ", line=" + std::to_string(line) + ")";
		return res;
	}

	Lexer::Lexer(const std::string& program)
		: program_(program), searchStr_(program), length_(program.length()) {
		std::cout << "Initialized with program: " << program_ << std::endl;

		std::string allGroups = "(";
		for (const auto& s : R_GROUPS) {
			allGroups += s;
			allGroups += ")|(";
		}

		groups_ = std::regex(allGroups.substr(0, allGroups.size() - 2));
		id_ = std::regex(R_ID);
		num_ = std::regex(R_NUM);

	}

	void Lexer::reset() {
		searchStr_ = program_;
		std::cout << "Reset to program_: " << program_ << std::endl;
	}

	Token::Type Lexer::getType(std::string match) {
		std::unordered_map<std::string, Token::Type>::const_iterator lookUp = TYPES.find(match);
		if (lookUp == TYPES.end()) {
			return Token::Type::undefined;
		}
		else {
			return lookUp->second;
		}
			
	}

	Token Lexer::nextToken() {
		return Token(Token::Type::undefined, "undefined", 0, 0);
	}


}
