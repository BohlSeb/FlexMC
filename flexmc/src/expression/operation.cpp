#include <stdexcept>
#include <cassert>
#include <format>
#include "operation.h"


namespace flexMC {

	// Refactor
	void functions::assertNumberOfArgs(const std::string& symbol, const int& min_args, const int& max_args, const std::size_t& num_args) {
		assert(((min_args <= max_args) || (max_args == -1)));
		std::string wrongArgs = "Function {0} takes between {1} and {2} arguments(s), got {3}";
		std::string wrongArgsEx = "Function {0} takes exactly {1} argument(s), got {2}";
		std::string noArgs = "Function {0}(...) takes no argument, got {1}";

		if ((min_args > 0) && (num_args < min_args)) {
			if (min_args == max_args) {
				auto msg = std::format(
					"Function {0} takes exactly {1} argument(s), got {2}", 
					symbol, min_args, num_args
				);
				throw std::runtime_error(msg);
			}
			auto msg = std::format(
				"Function {0} takes between {1} and {2} arguments(s), got {3}",
				symbol, min_args, max_args, num_args
			);;
			throw std::runtime_error(msg);
		}
		if ((0 <= max_args) && (max_args < num_args)) {
			if (max_args == 0) {
				auto msg = std::format("Function {0}(...) takes no argument, got {1}", symbol, num_args);
				throw std::runtime_error(msg);
			}
			if (max_args == min_args) {
				auto msg = std::format(
					"Function {0} takes exactly {1} argument(s), got {2}",
					symbol, min_args, num_args
				);
				throw std::runtime_error(msg);
			}
			auto msg = std::format(
				"Function {0} takes between {1} and {2} arguments(s), got {3}",
				symbol, min_args, max_args, num_args
			);
			throw std::runtime_error(msg);
		}
	}

}