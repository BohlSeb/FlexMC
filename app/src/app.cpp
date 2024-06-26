// flexmc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "app_config.h"

#include <ranges>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>

#include "lexer.h"
#include "expression_parser.h"
#include "terminals.h"

//#include "functions_real.h"
#include "expression_stacks.h"
#include "expression_compiler.h"
#include "operators_calc.h"
#include "expression_profiler.h"

using namespace flexMC;

int main()
{

    // report version
    std::cout << "Version: " << APP_VERSION_MAJOR << "."
              << APP_VERSION_MINOR << "."
              << APP_VERSION_PATCH << std::endl;

    scalarOperations();


    std::string test_program = "This is a program";


    const bool run_main = true;

    std::string test = std::string(0, '-');
    std::string at_test = std::string(4, '^');
    std::string end_test = std::string(10, '-');
    std::cout << test << at_test << end_test << "\n";

    if (run_main)
    {

//		const std::string program = " -2 * [0.0, 1.0, 2.0, 3, 2]";
        // const std::string program = "MAX(EXP(0)+5, 4, 0) + 3";
        const std::string program = "1 / EXP ([0.0, 1.0, 2.0] ) - 1";
        const std::string program_err = "2 + EXP( +3 )";
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
                std::cout << std::endl;
                // ExpressionCompiler compiler;
                Expression expression;
                const std::pair<const MaybeError, const CompileReport> reports
                        = ExpressionCompiler::compile(parsed, expression);

                if (reports.first.isError())
                {
                    std::cout << printError("Compilation", current, reports.first) << std::endl;
                }
                else
                {
                    Operands::Type return_type = reports.second.ret_type;

                    std::cout << "Compiled >> Return type: " << Operands::type2Str(return_type) << std::endl;

                    CalcStacks c_stacks(0, 0, 0, 0);

                    expression.evaluate(c_stacks);
                    if (c_stacks.size(Operands::Type::scalar) == 1)
                    {
                        std::cout << "Double result: " << c_stacks.scalarsBack() << std::endl;
                        c_stacks.popScalar();
                    }
                    else if (c_stacks.size(Operands::Type::vector) == 1)
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
