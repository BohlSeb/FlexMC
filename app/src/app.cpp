// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include "app_config.h"

#include <variant>
#include <unordered_map>

#include "lexer.h"
#include "expression_parser.h"
#include "terminals.h"
#include "expression_stacks.h"
#include "static_variables.h"
#include "expression_stacks.h"
#include "expression_compiler.h"
#include "operators_calc.h"
#include "expression_profiler.h"

using namespace flexMC;

template<class T>
struct MyMap {
    std::unordered_map<std::string, T> data;
};


int main()
{

    // report version
    std::cout << "Version: " << APP_VERSION_MAJOR << "."
              << APP_VERSION_MINOR << "."
              << APP_VERSION_PATCH << std::endl;



    StaticVStorage storage;

    SCALAR value = 1.0;
    const std::vector<double> values{1, 2, 3, 4};

    storage.insert<SCALAR>("x", value);
    storage.insert<VECTOR>("x", values);
    storage.insert<SCALAR>("x", 30.0);
    storage.insert<SCALAR>("z", 40.0);

    std::cout << cType2Str(storage.cType("x")) << std::endl;

    std::cout << storage.get<SCALAR>("x") << std::endl;
    std::cout << storage.get<SCALAR>("z") << std::endl;

    std::cout << std::boolalpha << storage.containsUnused() << std::endl;

//    storage.insert("x", 2);
//    storage.insert("y", 3);
//    storage.insert("z", 4);
//    storage.insert("myVec", values);

//    std::cout << cType2Str(storage.type("x")) << std::endl;
//
//    double value_ = storage.getScalar("x");
//
//    const std::vector<double> vector_used = storage.getVector("myVec");
//
//    std::cout << "Unused:" << std::endl;
//    for (const auto &[key, val]: storage.unused())
//    {
//        std::cout << key << ": " << "<" << cType2Str(val) << ">" << std::endl;
//    }
//    std::cout << "Unused end" << std::endl;
//
//
//    auto vector_used_final = storage.getVector("myVec");
//    std::cout << "Size: " << vector_used_final.size() << std::endl;


    const bool run_main = false;
    if (run_main)
    {

        /*
         * "3[(3]"
         */
        const std::string program = "1 / EXP ([0.0, 1.0, 2.0] ) - 1";
        const std::string program_err = "EXP(0.0)";
        auto current = program_err;
        std::cout << "Program to parse >>" << std::endl;
        std::cout << current << std::endl << std::endl;

        Lexer lexer;

        std::deque<Token> infix = lexer.tokenize(current);

        std::pair<MaybeError, std::vector<Token>> parse_result = infixToPostfix(infix);

        MaybeError parse_report = std::get<0>(parse_result);

        if (parse_report.isError())
        {
            std::cout << printError("Syntax", current, parse_report) << "\n";
        }
        else
        {

            auto parsed = parse_result.second;

            try
            {

                std::cout << "Items parsed: " << parsed.size() << std::endl;
                for (const auto &token: parsed)
                {
                    std::cout << token.toString() << " ";
                }
                std::cout << std::endl;
                Expression expression;
                const std::pair<const MaybeError, const CompileReport> reports = compileExpression(parsed, expression);

                if (reports.first.isError())
                {
                    std::cout << printError("Compilation", current, reports.first) << std::endl;
                }
                else
                {
                    CType return_type = reports.second.ret_type;

                    std::cout << "Compiled >> Return type: " << cType2Str(return_type) << std::endl;

                    CalcStacks c_stacks(0, 0, 0, 0);

                    expression.evaluate(c_stacks);
                    if (c_stacks.size(CType::scalar) == 1)
                    {
                        std::cout << "Double result: " << c_stacks.scalarsBack() << std::endl;
                        c_stacks.popScalar();
                    }
                    else if (c_stacks.size(CType::vector) == 1)
                    {
                        std::cout << "Vector result:" << std::endl << "[ ";
                        std::vector<double> res = c_stacks.vectorsBack();
                        for (auto r: res)
                        {
                            std::cout << r << ", ";
                        }
                        std::cout << "]" << std::endl;
                        c_stacks.popVector();
                    }
                    else
                    {
                        throw std::runtime_error("No result ready");
                    }
                    if (!c_stacks.ready())
                    {
                        throw std::runtime_error("Stacks not ready for next run");
                    }
                    std::cout << "flexmc calculated" << std::endl;
                }


            }
            catch (const std::runtime_error &error)
            {
                std::cout << "Error:" << std::endl;
                std::cout << error.what() << std::endl;
                //throw error;
            }


        }

        std::vector<Token> parsed = std::get<1>(parse_result);


//		auto parser = ExpressionParser(lexer);
//


    }

    std::cout << "Hello world" << std::endl;

    return 0;
}
