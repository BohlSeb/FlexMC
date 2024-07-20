#pragma once

#include <string_view>
#include <regex>
#include <deque>
#include <utility>

#include "tokens.h"

namespace flexMC
{

    class Lexer
    {

    public:

        Lexer();

        std::deque<Token> tokenize(const std::string_view line);

    private:

        std::pair<Token, std::string> nextTok(const std::string_view suffix, const std::size_t &line_no);

        std::regex groups_1_;
        std::regex groups_2_;
        std::regex id_;
        std::regex num_;
        std::match_results<std::string_view::const_iterator> match_;

    };
}
