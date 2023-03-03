#include <iostream>
#include <regex>
#include <unordered_map>
#include <stdexcept>

#include "lexer.h"
#include "language_constants.h"

namespace flexMC {

	std::string Token::type2String() {
		switch (type) {
		case Type::eof:
			return "endOfFile";
		case Type::eol:
			return "endOfLine";
		case Type::wsp:
			return "whiteSpace";
		case Type::tab:
			return "tab";
		case Type::sym:
			return "symbol";
		case Type::number:
			return "number";
		case Type::keyWord:
			return "keyWord";
		case Type::id:
			return "identifier";
		case Type::undefined:
			return "undefined";
		default:
			return "";
		}
	}

	std::string Token::toString() {
		std::string out;
		if ((type == Type::wsp) || (type == Type::eol) || (type == Type::tab) || (type == Type::eof)) {
			out = this->type2String();
		}
		else {
			out = value;
		}
		return "Token{" + type2String() + ", " + out + "}";
	}

	Lexer::Lexer(const std::string& program)
		: program_(program), searchStr_(program) {

		std::string allGroups = "(";
		for (const auto& s : R_GROUPS) {
			allGroups += s + ")|(";
		}

		groups_ = std::regex(allGroups.substr(0, allGroups.size() - 2));
		id_ = std::regex(R_ID);
		num_ = std::regex(R_NUM);

	}

	void Lexer::reset() {
		searchStr_ = program_;
	}

	Token::Type Lexer::getType(const std::string& match) {
		std::unordered_map<std::string, Token::Type>::const_iterator lookUp = TYPES.find(match);
		if (lookUp == TYPES.end()) {
			return Token::Type::undefined;
		}
		else {
			return lookUp->second;
		}

	}

	Token Lexer::nextToken() {

		if (searchStr_.empty()) {
			return Token(Token::Type::eof);
		}

		if (std::regex_search(searchStr_, match_, id_)) {
			Token res = Token(Token::Type::id, match_.str());
			searchStr_ = match_.suffix();
			return res;
		}

		if (std::regex_search(searchStr_, match_, num_)) {
			Token res = Token(Token::Type::number, match_.str());
			searchStr_ = match_.suffix();
			return res;
		}

		if (std::regex_search(searchStr_, match_, groups_)) {
			const std::string& value = match_.str();
			searchStr_ = match_.suffix();
			Token::Type type = getType(value);
			if (type == Token::Type::undefined) {
				throw std::runtime_error("undexptected match " + value + "preceding " + searchStr_);
			}
			else if (type == Token::Type::eol || type == Token::Type::wsp || type == Token::Type::tab) {
				return Token(type);
			}
			else {
				return Token(type, value);
			}
		}
		throw std::runtime_error("No match found at the beginning of " + searchStr_);
	}
}
