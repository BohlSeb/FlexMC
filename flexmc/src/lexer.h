#pragma once

#include <regex>

#include "tokens.h"

namespace flexMC {

	class Lexer {

	public:

		Lexer(const std::string& program);

		Token nextToken();

	private:

		std::string searchStr_;
		std::regex groups_1_;
		std::regex groups_2_;
		std::regex id_;
		std::regex num_;
		std::smatch match_;

	};
}
