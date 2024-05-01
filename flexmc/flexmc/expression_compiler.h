#pragma once
#include <string>
#include <vector>
#include <deque>

#include "tokens.h"
#include "expression_stacks.h"


namespace flexMC {

	class ExpressionCompiler {

	public:

		ExpressionCompiler(const std::deque<Token>& post_fix) : post_fix_(post_fix) { compile(); }

		const Operands::Type resultType() const { return return_type_; }

		// change to unique ptr and perform calculation within this class?
		std::vector<std::shared_ptr<PostFixItem>> compiled() { return result_; }

	private:

		void compile();

		const std::deque<Token>& post_fix_;

		Operands::Type return_type_;

		std::vector<std::shared_ptr<PostFixItem>> result_;
	
	};

}