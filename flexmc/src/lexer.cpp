#include <cassert>

#include "terminals.h"
#include "tokens.h"
#include "lexer.h"


namespace flexMC
{

    Lexer::Lexer()
    {
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

    std::deque<Token> Lexer::tokenize(const std::string_view line)
    {
        auto [token, suffix] = nextTok(line, 0);
        Token::Type previous = token.type;
        std::size_t line_no = token.size;
        std::deque<Token> out({token});

        while ((previous != Token::Type::eof) && (previous != Token::Type::undefined) && (line_no <= MAX_LINE_LEN))
        {
            auto [next, suffix_] = nextTok(suffix, line_no);
            previous = next.type;
            line_no += next.size;
            out.push_back(next);
            suffix = suffix_;
        }
        return out;
    }

    std::pair<Token, std::string> Lexer::nextTok(const std::string_view suffix, const std::size_t &line_no)
    {
        using
        enum Token::Type;

        if (suffix.empty())
        {
            return std::make_pair(Token(eof, "", line_no), "");
        }
        if (std::regex_search(suffix.cbegin(), suffix.cend(), match_, id_))
        {
            auto t = Token(id, match_.str(), line_no);
            return std::make_pair(t, match_.suffix());
        }

        if (std::regex_search(suffix.cbegin(), suffix.cend(), match_, num_))
        {
            auto t = Token(num, match_.str(), line_no);
            return std::make_pair(t, match_.suffix());
        }

        if (std::regex_search(suffix.cbegin(), suffix.cend(), match_, groups_1_))
        {
            return std::make_pair(Tokens::makeContextualized(match_.str(), line_no), match_.suffix());
        }

        if (std::regex_search(suffix.cbegin(), suffix.cend(), match_, groups_2_))
        {
            return std::make_pair(Tokens::makeContextualized(match_.str(), line_no), match_.suffix());
        }
        return std::make_pair(Token(undefined, suffix.data(), line_no), suffix.data());
    }

}
