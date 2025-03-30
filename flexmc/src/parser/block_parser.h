# pragma once

#include <vector>

#include "line_parser.h"


namespace flexMC {
    // struct BlockParseResult {
    //     const std::vector<LineParseResult> lines;
    // };

    // std::vector<BlockParseResult> parseBlocks(const std::vector<LineParseResult> &lines);

    std::pair<MaybeError, std::size_t> checkScopes(const std::vector<LineParseResult> &lines);

    namespace detail {
        struct CheckScopeState {
            std::size_t scope = 0;
            bool indent_required = false;
            bool terminate_called_in_branch = false;
        };
    }
}
