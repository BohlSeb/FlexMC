#pragma once

#include <vector>
#include <deque>

#include "tokens.h"
#include "language_error.h"


namespace flexMC
{

    std::pair<MaybeError, std::vector<Token>> infixToPostfix(const std::deque<Token> &infix);

}
