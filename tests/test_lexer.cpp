#include <gtest/gtest.h>
#include <ranges>

#include "lexer.h"


using namespace flexMC;


TEST(Lexer, ValidKeywordsAndSymbols) {

    Token::Type kW = Token::Type::keyword;
    Token::Type op = Token::Type::op;
    Token::Type fun = Token::Type::fun;
    Token::Type lparen = Token::Type::lparen;
    Token::Type lbracket = Token::Type::lbracket;
    Token::Type rparen = Token::Type::rparen;
    Token::Type rbracket = Token::Type::rbracket;

    std::vector<Token> expected = {
        Token(kW, "IF", 0),
        Token(kW, "CONTINUOUS", 0),
        Token(kW, "ELSE", 0),
        Token(fun, "PAY", 0),
        Token(fun, "PAY_AT", 0),
        Token(fun, "PAY", 0),
        Token(kW, "TERMINATE", 0),
        Token(kW, ":=", 0),
        Token(kW, "+=", 0),
        Token(kW, "-=", 0),
        Token(kW, "*=", 0),
        Token(kW, "/=", 0),
        Token(kW, "**=", 0),
        Token(fun, "EXP", 0),
        Token(fun, "LOG", 0),
        Token(fun, "ABS", 0),
        Token(fun, "SQRT", 0),
        Token(fun, "SQUARE", 0),
        Token(fun, "MIN", 0),
        Token(fun, "MAX", 0),
        Token(fun, "SUM", 0),
        Token(fun, "PROD", 0),
        Token(fun, "ARGMIN", 0),
        Token(fun, "ARGMAX", 0),
        Token(fun, "LEN", 0),
        Token(op, "AND", 0),
        Token(op, "OR", 0),
        Token(op, "NOT", 0),
        Token(op, "+", 0),
        Token(op, "-", 0),
        Token(op, "*", 0),
        Token(op, "/", 0),
        Token(op, "**", 0),
        Token(op, "<<", 0),
        Token(op, ">>", 0),
        Token(op, "<", 0),
        Token(op, ">", 0),
        Token(op, ",", 0),
        Token(lparen, "(", 0),
        Token(rparen, ")", 0),
        Token(op, "<=", 0),
        Token(op, ">=", 0),
        Token(lbracket, "[", 0),
        Token(rbracket, "]", 0),
        Token(Token::Type::eof, "", 0),
    };

    std::string testStr = "IF";
    testStr += "CONTINUOUS";
    testStr += "ELSE";
    testStr += "PAY";
    testStr += "PAY_AT";
    testStr += "PAY";
    testStr += "TERMINATE";
    testStr += ":=";
    testStr += "+=";
    testStr += "-=";
    testStr += "*=";
    testStr += "/=";
    testStr += "**=";
    testStr += "EXP";
    testStr += "LOG";
    testStr += "ABS";
    testStr += "SQRT";
    testStr += "SQUARE";
    testStr += "MIN";
    testStr += "MAX";
    testStr += "SUM";
    testStr += "PROD";
    testStr += "ARGMIN";
    testStr += "ARGMAX";
    testStr += "LEN";
    testStr += "AND";
    testStr += "OR";
    testStr += "NOT";
    testStr += "+";
    testStr += "-";
    testStr += "*";
    testStr += "/";
    testStr += "**";
    testStr += "<<";
    testStr += ">>";
    testStr += "<";
    testStr += ">";
    testStr += ",";
    testStr += "(";
    testStr += ")";
    testStr += "<=";
    testStr += ">=";
    testStr += "[";
    testStr += "]";


    Lexer lexer;
    std::deque<Token> tokens = lexer.tokenize(testStr);
    EXPECT_EQ(expected.size(), tokens.size());
    for (const auto &[exp, res]: std::ranges::zip_view(expected, tokens)) {
        EXPECT_EQ(exp.toString(), res.toString());
    }
}

TEST(Lexer, ValidNumbersAndIdentifiers) {

    Token::Type num = Token::Type::num;
    Token::Type id = Token::Type::id;
    Token::Type wsp = Token::Type::wsp;
    Token::Type op = Token::Type::op;
    Token::Type tab = Token::Type::tab;

    std::vector<Token> expected = {
        Token(id, "variable", 0),
        Token(wsp, " ", 0),
        Token(id, "_variable", 0),
        Token(wsp, " ", 0),
        Token(id, "_USER_CONSTANT", 0),
        Token(wsp, " ", 0),
        Token(id, "userNumber10", 0),
        Token(wsp, " ", 0),
        Token(id, "userVariable", 0),
        Token(wsp, " ", 0),
        Token(id, "min", 0),
        Token(wsp, " ", 0),
        Token(id, "userValue10Suffix", 0),
        Token(wsp, " ", 0),
        Token(id, "userValue_10_Suffix", 0),
        Token(wsp, " ", 0),
        Token(id, "_USER_CONSTANT_101", 0),
        Token(wsp, " ", 0),
        Token(id, "_USER_110_CONSTANT", 0),
        Token(wsp, " ", 0),
        Token(num, "0", 0),
        Token(wsp, " ", 0),
        Token(num, "0.05", 0),
        Token(wsp, " ", 0),
        Token(num, "100", 0),
        Token(wsp, " ", 0),
        Token(num, "921039112", 0),
        Token(wsp, " ", 0),
        Token(num, "100.3", 0),
        Token(wsp, " ", 0),
        Token(num, "1e3", 0),
        Token(wsp, " ", 0),
        Token(num, "1.02E3", 0),
        Token(wsp, " ", 0),
        Token(num, "1.02E-3", 0),
        Token(wsp, " ", 0),
        Token(num, "4e-5", 0),
        Token(wsp, " ", 0),
        Token(num, "1.", 0),
        Token(wsp, " ", 0),
        Token(num, ".555", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "0", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "0.0000", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "0.005", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "100.3", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "1e3", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "1.02E3", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "921039112", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "4e-5", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, "1.", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(num, ".555", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(wsp, " ", 0),
        Token(num, ".555", 0),
        Token(wsp, " ", 0),
        Token(op, "-", 0),
        Token(wsp, " ", 0),
        Token(num, "1.", 0),
        Token(tab, "    ", 0),
        Token(op, "-", 0),
        Token(tab, "    ", 0),
        Token(num, ".555", 0),
        Token(tab, "    ", 0),
        Token(op, "-", 0),
        Token(tab, "    ", 0),
        Token(num, "1.", 0),
        Token(Token::Type::eof, "", 0),
    };

    std::string testStr = "variable";
    testStr += " ";
    testStr += "_variable";
    testStr += " ";
    testStr += "_USER_CONSTANT";
    testStr += " ";
    testStr += "userNumber10";
    testStr += " ";
    testStr += "userVariable";
    testStr += " ";
    testStr += "min";
    testStr += " ";
    testStr += "userValue10Suffix";
    testStr += " ";
    testStr += "userValue_10_Suffix";
    testStr += " ";
    testStr += "_USER_CONSTANT_101";
    testStr += " ";
    testStr += "_USER_110_CONSTANT";
    testStr += " ";
    testStr += "0";
    testStr += " ";
    testStr += "0.05";
    testStr += " ";
    testStr += "100";
    testStr += " ";
    testStr += "921039112";
    testStr += " ";
    testStr += "100.3";
    testStr += " ";
    testStr += "1e3";
    testStr += " ";
    testStr += "1.02E3";
    testStr += " ";
    testStr += "1.02E-3";
    testStr += " ";
    testStr += "4e-5";
    testStr += " ";
    testStr += "1.";
    testStr += " ";
    testStr += ".555";
    testStr += " ";
    testStr += "-0";
    testStr += " ";
    testStr += "-0.0000";
    testStr += " ";
    testStr += "-0.005";
    testStr += " ";
    testStr += "-100.3";
    testStr += " ";
    testStr += "-1e3";
    testStr += " ";
    testStr += "-1.02E3";
    testStr += " ";
    testStr += "-921039112";
    testStr += " ";
    testStr += "-4e-5";
    testStr += " ";
    testStr += "-1.";
    testStr += " ";
    testStr += "-.555";
    testStr += " ";
    testStr += "- .555";
    testStr += " ";
    testStr += "- 1.";
    testStr += "    ";
    testStr += "-\t.555";
    testStr += "\t";
    testStr += "-    1.";

    Lexer lexer;
    std::deque<Token> tokens = lexer.tokenize(testStr);
    EXPECT_EQ(expected.size(), tokens.size());
    for (const auto &[exp, res]: std::ranges::zip_view(expected, tokens)) {
        EXPECT_EQ(exp.toString(), res.toString());
    }
};

