#pragma once
#include <string>
#include <vector>
#include <deque>

#include "tokens.h"
#include "expression_stacks.h"


namespace flexMC {

	class Expression {

	public:

		template <class T>
		inline void addItem(T item) { items_.emplace_back(std::make_unique<T>(std::move(item))); }

		void evaluate(CalcStacks& stacks) const;

	private:

		std::vector<std::unique_ptr<PostFixItem>> items_;

	};

	struct ExpressionCompiler {

		const Operands::CompileReport compile(const std::deque<Token>& post_fix, Expression& expression);

	};

}