#pragma once

#include <string>
#include <array>

namespace flexMC
{

    // Keywords
    const std::string PAY = "PAY";
    const std::string STOP = "STOP";
    const std::string IF = "IF";
    const std::string THEN = "THEN";
    const std::string ELSE = "ELSE";
    const std::string ASSIGN = ":=";
    const std::string PLUS_ASSIGN = "+=";
    const std::string MINUS_ASSIGN = "-=";
    const std::string MUL_ASSIGN = "*=";
    const std::string DIV_ASSIGN = "/=";
    const std::string POW_ASSIGN = "**=";

    // Real functions taking a scalar or vector
    const std::string EXP = "EXP";
    const std::string LOG = "LOG";
    const std::string ABS = "ABS";
    const std::string SQRT = "SQRT";
    const std::string SQUARE = "SQUARE";

    // Real functions taking a vector or positive number of reals
    const std::string MIN = "MIN";
    const std::string MAX = "MAX";
    const std::string SUM = "SUM";
    const std::string PROD = "PROD";
    const std::string ARGMIN = "ARGMIN";
    const std::string ARGMAX = "ARGMAX";

    // Real functions taking a vector
    const std::string LEN = "LEN";

    // Operators
    const std::string AND = "AND";
    const std::string OR = "OR";
    const std::string NOT = "NOT";
    const std::string PLUS = "+";
    const std::string MINUS = "-";
    const std::string MUL = "*";
    const std::string DIV = "/";
    const std::string POW = "**";
    const std::string LT = "<";
    const std::string GT = ">";
    const std::string LE = "<=";
    const std::string GE = ">=";
    const std::string SMOOTH_LT = "<<";
    const std::string SMOOTH_GT = ">>";
    const std::string COMMA = ",";
    const std::string P_LEFT = "(";
    const std::string P_RIGHT = ")";
    const std::string B_LEFT = "[";
    const std::string B_RIGHT = "]";

    // Internal function call and list append operators
    const std::string CALL_ = "CALL_";
    const std::string APPEND_ = "APPEND_";
    const std::string INDEX_ = "INDEX_";

    // Identifiers must start with either a lower case letter or "_"
    const std::string R_ID = R"(^[_a-z]\w*)";
    const std::string R_NUM = R"(^(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?)";

    const std::array<std::string, 2> R_GROUPS_1 = {
            R"(^\*\*(=?))",
            R"(^\s{4})",
    };

    const std::array<std::string, 35> R_GROUPS_2 = {
            R"(^IF)",
            R"(^THEN)",
            R"(^ELSE)",
            R"(^AND)",
            R"(^OR)",
            R"(^NOT)",
            R"(^PAY)",
            R"(^STOP)",
            R"(^EXP)",
            R"(^LOG)",
            R"(^ABS)",
            R"(^SQRT)",
            R"(^SQUARE)",
            R"(^MIN)",
            R"(^MAX)",
            R"(^SUM)",
            R"(^PROD)",
            R"(^ARGMIN)",
            R"(^ARGMAX)",
            R"(^LEN)",
            R"(^APPEND)",
            R"(^:=)",
            R"(^\+(=?))",
            R"(^\-(=?))",
            R"(^\*(=?))",
            R"(^\/(=?))",
            R"(^\<[\<=]?)",
            R"(^\>[\>=]?)",
            R"(^\,)",
            R"(^\()",
            R"(^\))",
            R"(^\[)",
            R"(^\])",
            R"(^\t)",
            R"(^\s)",
    };

    const std::size_t MAX_LINE_LEN = 1000;
};
