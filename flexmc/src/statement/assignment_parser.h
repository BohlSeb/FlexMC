#pragma once

#include <deque>

#include "tokens.h"
#include "language_error.h"


namespace flexMC
{

    std::pair<MaybeError, std::deque<Token>> parseLineStart(const std::deque<Token> &line);

}