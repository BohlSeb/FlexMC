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

    const bool run_main = true;

    std::vector<int> t_1{1, 2, 3};
    std::vector<int> t_2{4, 5, 6};

    for (const std::pair<const int&, const int&> elem : std::views::zip(t_1, t_1)){
        std::cout << std::get<0>(elem) << "\n";
    }

    if (run_main)
    {

//		const std::string program = " -2 * [0.0, 1.0, 2.0, 3, 2]";
        // const std::string program = "MAX(EXP(0)+5, 4, 0) + 3";
        const std::string program = "EXP ([0.0, 1.0, 2.0] ) - 1";
        std::cout << "Program to parse >>" << std::endl;
        std::cout << program << std::endl << std::endl;

        Lexer lexer;

        std::deque<Token> infix = lexer.tokenize(program);

        std::pair<MaybeError, std::vector<Token>> parse_result = postfix(infix);

        MaybeError parse_report = std::get<0>(parse_result);
        std::vector<Token> parsed = std::get<1>(parse_result);


//		auto parser = ExpressionParser(lexer);
//
        try
        {

            std::cout << "Items parsed: " << parsed.size() << std::endl;
            for (auto &iter: std::ranges::reverse_view(parsed))
            {
                std::cout << iter.value << " ";
            }
            std::cout << std::endl;
            // ExpressionCompiler compiler;
            Expression expression;
            auto report = ExpressionCompiler::compile(parsed, expression);
            std::cout << "flexmc compiled" << std::endl;

            Operands::Type return_type = report.ret_type;
            std::cout << "Return type: " << Operands::type2Str(return_type) << std::endl;

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
        catch (const std::runtime_error &error)
        {
            std::cout << "Error:" << std::endl;
            std::cout << error.what() << std::endl;
            //throw error;
        }

    }

    std::cout << "Hello world" << std::endl;

    return 0;
}
