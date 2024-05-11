#include <stdexcept>
#include <cassert>
#include <vector>
#include <string>
//#include <format>
#include "operation.h"


namespace flexMC {

	void functions::assertNumberOfArgs(const int& min_args, const int& max_args, const std::size_t& num_args) {
		assert(((min_args <= max_args) || (max_args == -1)));
		std::string wrongArgs = "takes between {} and {} arguments(s), got {}";
		std::string wrongArgsEx = "takes exactly {} argument(s), got {}";
		std::string noArgs = "Function {}(...) takes no argument, got {}";

		if ((min_args > 0) && (num_args < min_args)) {
			if (min_args == max_args) {
				std::string problem = wrongArgsEx;
				throw std::runtime_error(problem);
			}
			std::string problem = wrongArgs;
			throw std::runtime_error(problem);
		}
		if ((0 <= max_args) && (max_args < num_args)) {
			if (max_args == 0) {
				std::string problem = noArgs;
				throw std::runtime_error(problem);
			}
			if (max_args == min_args) {
				std::string problem = wrongArgsEx;
				throw std::runtime_error(problem);
			}
			std::string problem = wrongArgs;
			throw std::runtime_error(problem);
		}
	}

}