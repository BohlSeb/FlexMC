// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include "app_config.h"

#include "lexer.h"
#include "tokens.h"
#include "statement_parser.h"


using namespace flexMC;


int main()
{

    // report version
    std::cout << "Version: " << APP_VERSION_MAJOR << "."
              << APP_VERSION_MINOR << "."
              << APP_VERSION_PATCH << std::endl;

//    std::string program = "myVariableDate PAY *= ";
//
//    Lexer l;
//    std::deque<Token> infix = l.tokenize(program);
//
//    const auto [parse_report, line_start] = splitLine(infix);
//
//    std::cout << std::boolalpha << parse_report.isError() << "\n";
//    if (parse_report.isError()) {
//        std::cout << printError("Parse Error", program, parse_report) << "\n";
//    }
//    std::cout << line_start.size() << "\n";

    std::vector<int> vec = {4, 2, 4, 5, 2, 1, 1};

    auto assign_l = [](int x)
    { return x == 5; };

    auto paren_l = [](int x)
    { return x >= 4; };


    auto it_5 = std::ranges::find_if(vec.begin(), vec.end(), assign_l);
    auto it_test = std::ranges::find_last_if(vec.begin(), it_5, paren_l).begin();

    if (it_test != it_5)
    {
        std::cout << *it_test << "\n";
    } else {
        std::cout << "not found\n";
    }

    vec.insert(it_5, 7);
    //vec.erase(it_test, it_5);

    for (auto &v: vec)
    {
        std::cout << v << " ";
    }


    std::string nothing;
    std::cout << std::boolalpha << nothing.empty() << "\n";



//    std::cout << "it_5 " << *it_5 << "\n";
//    std::cout << "rit: " << *rit << "\n";
//    auto rit_2 = std::ranges::find_if(rit, rit_end, [](int x)
//    { return x == 6; });
//
//    for (auto _it = rit; _it != rit_2; ++_it)
//    {
//        std::cout << *_it << " ";
//    }


}
