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

		Token(Type type) : type(type), value("") {
		}

		Token(Type type, const std::string& value)
			: type(type), value(value) {
		}

		std::string toString() const;
		std::string type2String() const;

		const Type type;
		const std::string value;

	};

	std::ostream& operator<<(std::ostream& output, const Token& token);

	class Lexer {

	public:

		Lexer(const std::string& program);

		Token nextToken();

		void reset();

	private:

		Token::Type getType(const std::string& match) const;

		const std::string program_;
		std::string searchStr_;

		std::regex groups_;
		std::regex id_;
		std::regex num_;
		std::smatch match_;

		int errors_;

	};

}


