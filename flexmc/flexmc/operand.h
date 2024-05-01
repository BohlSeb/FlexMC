#pragma once

#include <string>
#include "expression_stacks.h"


// Todo: Keep precision in mind

namespace flexMC {

	class Number final : public PostFixItem {

	public:

		Number(const std::string& symbol) : value_(std::stod(symbol)) {}

		void compile(Operands& stacks);

		void evaluate(CalcStacks& stacks) override;

	private:

		const double value_;

	};

	class Vector final : public PostFixItem {
		
	public:

		Vector(const int& size) : size_(size) {}

		void evaluate(CalcStacks& stacks) override;

	private:

		const int size_;

	};

}
