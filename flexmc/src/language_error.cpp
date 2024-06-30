#include <cassert>
#include <sstream>
#include <fmt/format.h>

#include "language_error.h"

namespace flexMC
{
    void MaybeError::setError(const std::string &msg, const size_t &at, const size_t &len)
    {
        assert(!isError());
        err_msg_ = msg;
        err_at_ = at;
        err_len_ = len;
    }


    void MaybeError::setMessage(const std::string &msg)
    {
        assert(!isError());
        err_msg_ = msg;
    }


    void MaybeError::setPosition(const size_t &at, const size_t &len)
    {
        assert(isError());
        err_at_ = at;
        err_len_ = len;
    }


    std::string printError(const std::string err_prefix, const std::string &line, const MaybeError &report)
    {
        assert(report.isError());
        std::stringstream out;
        const std::pair<const size_t &, const size_t &> pos = report.position();
        auto start = pos.first;
        auto len = std::max<size_t>(pos.second, 1);
        out << fmt::format("{} Error in line:\n", err_prefix);
        out << fmt::format("\"{}\"", line) << "\n";
        out << " " << std::string(start, ' ') << std::string(len, '^') << "\n";
        out << report.msg() << '.';
        return out.str();
    }
}