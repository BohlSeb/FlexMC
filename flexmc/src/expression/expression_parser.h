#pragma once

#include <sstream>
#include <deque>
#include <utility>

#include "tokens.h"
#include "lexer.h"


namespace flexMC
{


    class MaybeError
    {

    public:

        MaybeError() : err_msg_(""), err_at_(0), err_len_(0)
        {}

        void setError(const std::string &msg, const size_t &at, const size_t &len);

        bool isError() const
        { return !err_msg_.empty(); }

        std::pair<size_t, size_t> at() const
        { return std::make_pair(err_at_, err_len_); }

        std::string msg() const
        { return err_msg_; }

    private:

        std::string err_msg_;

        size_t err_at_;

        size_t err_len_;

    };

    std::pair<MaybeError, std::vector<Token>> postfix(const std::deque<Token> &infix);

}
