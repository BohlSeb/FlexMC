#include <cassert>
#include <fmt/format.h>

#include "operation.h"


namespace flexMC
{

    void functions::assertNumberOfArgs(const Token &function,
                                       const size_t &expected,
                                       const size_t &num_args,
                                       const Operands::Type &arg_type,
                                       MaybeError &report)
    {
        if (expected != num_args)
        {
            auto msg = fmt::format(R"(Function "{}" with argument type <{}> takes exactly {} argument(s), got {})",
                                   function.value, Operands::type2Str(arg_type), expected, num_args);
            report.setError(msg, function.start, function.size);
        }
    }

    void functions::assertNumberOfArgs(const Token &function,
                                       const size_t &min_args,
                                       const size_t &max_args,
                                       const size_t &num_args,
                                       const Operands::Type &arg_type,
                                       MaybeError &report)
    {
        if ((num_args < min_args) || ((max_args > 0) && (num_args > max_args)))
        {
            std::string msg;
            if (max_args > 0)
            {
                msg = fmt::format(
                        R"(Function "{}" with argument type <{}> takes between {} and {} argument(s), got {})",
                        function.value, Operands::type2Str(arg_type), min_args, max_args, num_args
                );
            }
            else
            {
                msg = fmt::format(
                        R"(Function "{}" with argument type <{}> takes at least {} argument(s), got {})",
                        function.value, Operands::type2Str(arg_type), min_args, num_args
                );
            }
            report.setError(msg, function.start, function.size);
        }

    }

}