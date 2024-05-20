#pragma once

#include <string>

#include "expression_stacks.h"


// Todo: Keep precision in mind

namespace flexMC {

	class Number final : public PostFixItem {

	public:

		explicit Number(const std::string& symbol) : value_(std::stod(symbol)) {}

		static void compile(Operands& stacks);

		void evaluate(CalcStacks& stacks) override;

	private:

		const double value_;

	};

	class Vector final : public PostFixItem {
		
	public:

		explicit Vector(const size_t& size) : size_(size) {}

		void evaluate(CalcStacks& stacks) override;

	private:

		const size_t size_;

	};

}
