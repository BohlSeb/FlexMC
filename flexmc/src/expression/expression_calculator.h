#pragma once

#include <sstream>
#include <stdexcept>

#include "expression_parser.h"
#include "expression_compiler.h"


namespace flexMC
{

    class StaticExpressionCalculator
    {

    public:

        explicit StaticExpressionCalculator(const std::string &expression_str) : postfix_(initialize(expression_str)),
                                                                                 infix_(expression_str)
        {}

        template<class T>
        void setVariable(const std::string &variable_name, const T &value)
        {
            is_compiled_ = false;
            constexpr CType c_type = getCType<T>();
            if (storage_.contains(variable_name) && storage_.cType(variable_name) != c_type)
            {
                std::stringstream msg{"Variable Error (already defined): Cannot set "};
                msg << variable_name << " from type " << cType2Str(storage_.cType(variable_name)) << " to ";
                msg << cType2Str(c_type);
                throw std::runtime_error(msg.str());
            }
            storage_.insert<T>(variable_name, value);
        }

        SCALAR calculateScalar();

        VECTOR calculateVector();

        void checkAndCalculate(const CType& requested_return_type);

        // void clearUnusedVariables();

    private:

        std::vector<Token> initialize(const std::string &infix);

        void compile();

        const std::vector<Token> postfix_;

        const std::string infix_;

        StaticVStorage storage_;

        Expression expression_;

        CalcStacks stacks_{0, 0, 0, 0};  // bad (size of the compiled expression is not yet known)

        CType return_type_ = CType::undefined; // bad, should be const

        bool is_compiled_ = false;

    };

}
