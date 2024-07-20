#include <cassert>
#include <sstream>
#include <string_view>
#include <fmt/format.h>

#include "language_error.h"

namespace flexMC
{
    void MaybeError::setError(const std::string_view msg, const std::size_t &at, const std::size_t &len)
    {
        assert(!isError());
        err_msg_ = msg;
        err_at_ = at;
        err_len_ = len;
    }


    void MaybeError::setMessage(const std::string_view msg)
    {
        assert(!isError());
        err_msg_ = msg;
    }


    void MaybeError::setPosition(const std::size_t &at, const std::size_t &len)
    {
        assert(isError());
        err_at_ = at;
        err_len_ = len;
    }


    std::string printError(std::string_view err_prefix, std::string_view line, const MaybeError &report)
    {
        assert(report.isError());
        std::stringstream out;
        auto [start, size] = report.position();
        auto len = std::max<size_t>(size, 1);
        out << fmt::format("{} Error in line:\n", err_prefix);
        out << fmt::format("\"{}\"", line) << "\n";
        out << " " << std::string(start, ' ') << std::string(len, '^') << "\n";
        out << report.msg() << '.';
        return out.str();
    }
}