#pragma once

#include <string_view>
#include <unordered_map>

// suggested by SonarLint cpp:S6045 to use for string keys in std::unordered_map
// "Transparent function objects should be used with associative "std::string" containers"
// https://rules.sonarsource.com/cpp/tag/since-c++14/RSPEC-6045/


namespace flexMC {

    struct SHash {
        using is_transparent [[maybe_unused]] = void;

        std::size_t operator()(const std::string_view s_view) const { return std::hash<std::string_view>{}(s_view); }
    };

    template<class T>
    using StringMap = std::unordered_map<std::string, T, SHash, std::equal_to<>>;

}
