#pragma once

#include <string_view>


struct SHash
{

    using is_transparent [[maybe_unused]] = void;

    inline size_t operator()(const std::string_view s_view) const
    { return std::hash<std::string_view>{}(s_view); }

};