#include <iostream>
#include <regex>
#include <unordered_map>
#include <stdexcept>

#include "lexer.h"
#include "terminals.h"
#include "tokens.h"


namespace flexMC {

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
			Token res = Token(Token::Type::num, match_.str());
			searchStr_ = match_.suffix();
			return res;
		}

		if (std::regex_search(searchStr_, match_, groups_)) {
			const std::string& value = match_.str();
			searchStr_ = match_.suffix();
			return Tokens::makeContextualized(value);
		}
		return Token(Token::Type::undefined, "regex");
	}
}
