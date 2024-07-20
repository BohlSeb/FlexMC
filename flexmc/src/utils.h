#pragma once

#include <string_view>
#include <unordered_map>

namespace flexMC
{

    struct SHash
    {
        using is_transparent [[maybe_unused]] = void;

        inline size_t operator()(const std::string_view s_view) const
        { return std::hash<std::string_view>{}(s_view); }
    };

    template<class T>
    using StringMap = std::unordered_map<std::string, T, SHash, std::equal_to<>>;

}
