#pragma once

#include <iostream>
#include <regex>

namespace flexMC {

	struct Token {

		enum class Type {
			eof,
			eol,
			wsp,
			tab,
			sym,
			number,
			keyWord,
			id,
			undefined
		};

		Token(Type type, const std::string& value, int position, int line)
			: type(type), value(value), position(position), line(line) {
		}



		std::string toString();
		std::string type2String();

		const Type type;
		const std::string value;
		const int position;
		const int line;

	};

	class Lexer {

	public:

		Lexer(const std::string& program);

		Token nextToken();

		void reset();

	private:

		Token::Type getType(std::string match);

		const std::string program_;
		std::string searchStr_;
		const int length_;

		std::regex groups_;
		std::regex id_;
		std::regex num_;
		std::smatch match_;
		
	};

}


