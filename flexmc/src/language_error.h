# pragma once

#include <utility>
#include <string>
#include <string_view>

#include "tokens.h"

namespace flexMC {

    class MaybeError {

    public:

        void setError(const std::string_view msg, const Token &token);

        void setError(const std::string_view msg, const std::size_t &at, const std::size_t &len);

        bool isError() const { return !err_msg_.empty(); }

        void setMessage(const std::string_view msg);

        void setPosition(const std::size_t &at, const std::size_t &len);

        std::pair<size_t, size_t> position() const { return std::make_pair(err_at_, err_len_); }

        std::string msg() const { return err_msg_; }

    private:

        std::string err_msg_;

        std::size_t err_at_{0};

        std::size_t err_len_{0};

    };

    std::string printError(const std::string_view err_prefix,
                           const std::string_view line,
                           const MaybeError &report); // line_no default 0

}