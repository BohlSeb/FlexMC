#include <cassert>

#include "terminals.h"
#include "tokens.h"
#include "lexer.h"


namespace flexMC
{

    Lexer::Lexer() {

        std::string all_groups_1 = "(";
        for (const auto &s: R_GROUPS_1)
        {
            all_groups_1 += s + ")|(";
        }

        std::string all_groups_2 = "(";
        for (const auto &s: R_GROUPS_2)
        {
            all_groups_2 += s + ")|(";
        }

        groups_1_ = std::regex(all_groups_1.substr(0, all_groups_1.size() - 2));
        groups_2_ = std::regex(all_groups_2.substr(0, all_groups_2.size() - 2));
        id_ = std::regex(R_ID);
        num_ = std::regex(R_NUM);
    }


    std::deque<Token> Lexer::tokenize(const std::string &line)
    {
        const size_t max_line_length = 1000;  // make a setting

        std::pair<Token, std::string> first_pair = nextTok(line, 0);
        std::string suffix = std::get<1>(first_pair);
        Token first = std::get<0>(first_pair);
        Token::Type previous = first.type;
        size_t line_no = first.size;
        std::deque<Token> out({first});

        while ((previous != Token::Type::eof) && (previous != Token::Type::undefined) && (line_no <= max_line_length))
        {
            std::tuple<Token, std::string> pair = nextTok(suffix, 0);
            suffix = std::get<1>(pair);
            Token next = std::get<0>(pair);
            previous = next.type;
            line_no += next.size;
            out.push_back(next);
        }
        return out;
    }


    std::pair<Token, std::string> Lexer::nextTok(const std::string &suffix, const size_t &line_no)
    {
        using type = Token::Type;

        if (suffix.empty())
        {
            return std::make_pair(Token(type::eof, "", line_no), "");
        }
        if (std::regex_search(suffix, match_, id_))
        {
            // strange experiences with results of match_.someFun()
            const std::string value_ = match_.str();
            const std::string suffix_ = match_.suffix();
            auto t = Token(type::id, match_.str(), line_no);
            return std::make_pair(t, suffix_);
        }

        if (std::regex_search(suffix, match_, num_))
        {
            const std::string value_ = match_.str();
            const std::string suffix_ = match_.suffix();
            auto t = Token(type::num, value_, line_no);
            return std::make_pair(t, suffix_);
        }

        if (std::regex_search(suffix, match_, groups_1_))
        {
            const std::string value_ = match_.str();
            const std::string suffix_ = match_.suffix();
            return std::make_pair(Tokens::makeContextualized(value_, line_no), suffix_);
        }

        if (std::regex_search(suffix, match_, groups_2_))
        {
            const std::string value_ = match_.str();
            const std::string suffix_ = match_.suffix();
            return std::make_pair(Tokens::makeContextualized(value_, line_no), suffix_);
        }
        return std::make_pair(Token(type::undefined, suffix, line_no), suffix);
    }

}
