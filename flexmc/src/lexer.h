#pragma once

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

        std::deque<Token> tokenize(const std::string &line);

    private:

        std::pair<Token, std::string> nextTok(const std::string &suffix, const size_t &line_no);

        std::regex groups_1_;
        std::regex groups_2_;
        std::regex id_;
        std::regex num_;

        std::smatch match_;

    };
}
