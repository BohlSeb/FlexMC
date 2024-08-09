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

using namespace flexMC;


template<class scalar_function>
void calculateVector(std::vector<double> &vectors, const std::size_t size, scalar_function f)
{
    assert(vectors.size() >= size);
    std::ranges::transform(vectors.end() - size, vectors.end(), vectors.end() - size, f);
}


int main()
{

    // report version
    std::cout << "Version: " << APP_VERSION_MAJOR << "."
              << APP_VERSION_MINOR << "."
              << APP_VERSION_PATCH << std::endl;

    StaticVStorage storage;

    SCALAR value = 1.0;
//    const std::vector<double> values{1, 2, 3, 4};

    storage.insert<SCALAR>("x", value);
//    storage.insert<VECTOR>("x", values);
    storage.insert<VECTOR>("x", {30.0, 40.0});
//    storage.insert<SCALAR>("z", 40.0);
//    storage.insert<SCALAR>("x", 5);

    std::vector<double> values{1, 2, 3, 4};

    const auto f = [](const SCALAR &value)
    { return value * value; };

    std::cout << "Values before" << "\n";

    for (const auto &v: values)
    {
        std::cout << v << " ";
    }
    std::cout << "\n";

    calculateVector(values, 4, f);

    std::cout << "Values before" << "\n";

    for (const auto &v: values)
    {
        std::cout << v << " ";
    }
    std::cout << "\n";

    const bool run_main = true;

    if (run_main)
    {

        /*
         * "3[(3]"
         */
        const std::string program = "1 / EXP ([0.0, 1.0, 2.0] ) - 1";
        const std::string program_err = "(-1, -1) - (1, 1)";
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
                    std::cout << token.value << " ";
                }
                std::cout << "\n";
                Expression expression;
                const std::pair<const MaybeError, const CompileReport> reports = compileExpression(parsed, expression,
                                                                                                   storage);

                if (reports.first.isError())
                {
                    std::cout << printError("Compilation", current, reports.first) << std::endl;
                }
                else
                {
                    CType return_type = reports.second.ret_type;

                    std::cout << "Compiled >> Return type: " << cType2Str(return_type) << std::endl;

                    CalcStacks c_stacks(0, 0, 0, 0);

                    expression(c_stacks);
                    if (c_stacks.size(CType::scalar) == 1)
                    {
                        std::cout << "Double result: " << c_stacks.scalars().back() << std::endl;
                        c_stacks.scalars().pop_back();
                    }
                    else if (c_stacks.vectorSizes().size() == 1)
                    {
                        std::cout << "Vector result:" << std::endl << "[ ";
                        std::vector<double> res = c_stacks.vectorResult();
                        for (auto r: res)
                        {
                            std::cout << r << ", ";
                        }
                        std::cout << "]" << std::endl;
                        c_stacks.popVectorResult();
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
                std::cout << "Error 2:" << std::endl;
                std::cout << error.what() << std::endl;
            }


        }

        std::vector<Token> parsed = std::get<1>(parse_result);

    }

    std::cout << "Hello world" << std::endl;

    return 0;
}
