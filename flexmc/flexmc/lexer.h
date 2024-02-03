#pragma once

#include <iostream>
#include <regex>

#include "tokens.h"

namespace flexMC {

	class Lexer {

	public:

		Lexer(const std::string& program);

		Token nextToken();

	private:

		std::string searchStr_;
		std::regex groups_;
		std::regex id_;
		std::regex num_;
		std::smatch match_;

	};
}
