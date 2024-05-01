#include <iostream>
#include <regex>
#include <unordered_map>

#include "terminals.h"
#include "tokens.h"

#include "lexer.h"


namespace flexMC {

	Lexer::Lexer(const std::string& program) :
		searchStr_(program) {

		std::string all_groups_1 = "(";
		for (const auto& s : R_GROUPS_1) {
			all_groups_1 += s + ")|(";
		}

		std::string all_groups_2 = "(";
		for (const auto& s : R_GROUPS_2) {
			all_groups_2 += s + ")|(";
		}

		groups_1_ = std::regex(all_groups_1.substr(0, all_groups_1.size() - 2));
		groups_2_ = std::regex(all_groups_2.substr(0, all_groups_2.size() - 2));
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

		if (std::regex_search(searchStr_, match_, groups_1_)) {
			const std::string& value = match_.str();
			searchStr_ = match_.suffix();
			return Tokens::makeContextualized(value);
		}

		if (std::regex_search(searchStr_, match_, groups_2_)) {
			const std::string& value = match_.str();
			searchStr_ = match_.suffix();
			return Tokens::makeContextualized(value);
		}
		return Token(Token::Type::undefined, "regex");
	}


}
