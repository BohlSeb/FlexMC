#pragma once

#include <string>
#include <functional>
#include "expression_stacks.h"

namespace flexMC {

	namespace functions {

		void assertNumberOfArgs(const int& min_args, const int& max_args, const std::size_t& num_args);

	}

	class Operation : public PostFixItem {

	public:

		Operation(const std::function<void(CalcStacks& stacks)>& call_back) : call_back_(call_back) {}

		inline void evaluate(CalcStacks& stacks) override { call_back_(stacks); }

	private:

		const std::function<void(CalcStacks& stacks)> call_back_;

	};

}
