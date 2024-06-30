#include <cassert>
#include <fmt/format.h>

#include "operation.h"


namespace flexMC
{

    void functions::assertNumberOfArgs(const Token& function,
                            const size_t &expected,
                            const size_t &num_args,
                            MaybeError &report)
    {
        if (expected != num_args)
        {
            auto msg = fmt::format("Function \"{0}\" takes exactly {1} argument(s), got {2}",
                                   function.value, expected, num_args);
            report.setError(msg, function.start, function.size);
        }
    }

    void functions::assertNumberOfArgs(const Token &function,
                                       const size_t &min_args,
                                       const size_t &max_args,
                                       const size_t &num_args,
                                       MaybeError &report)
    {
        if ((num_args < min_args) || ((max_args > 0) && (num_args > max_args)))
        {
            auto msg = fmt::format(
                    "Function {0} takes between {1} and {2} arguments(s), got {3}",
                    function.value, min_args, max_args, num_args
            );
            report.setError(msg, function.start, function.size);
        }

    }

}