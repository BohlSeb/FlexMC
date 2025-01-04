// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "app_config.h"

#include "lexer.h"
#include "tokens.h"
#include "assignment_parser.h"


using namespace flexMC;




int main()
{

    // report version
    std::cout << "Version: " << APP_VERSION_MAJOR << "."
              << APP_VERSION_MINOR << "."
              << APP_VERSION_PATCH << std::endl;

    std::string program = "    myVar += ";

    Lexer l;
    std::deque<Token> infix = l.tokenize(program);

    const auto [parse_report, line_start] = parseLineStart(infix);

    std::cout << std::boolalpha << parse_report.isError() << "\n";
    if (parse_report.isError()) {
        std::cout << printError("Parse Error", program, parse_report) << "\n";
    }
    std::cout << line_start.size() << "\n";



}
