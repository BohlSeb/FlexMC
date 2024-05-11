#pragma once

#include <unordered_map>
#include <functional>
#include <cmath>

#include "terminals.h"
#include "operation.h"

namespace flexMC {

	namespace operatorsCalc {

		const bool isBinarySymbol(const std::string& symbol);

		std::function<void(CalcStacks&)> compileBinary(const std::string& symbol, Operands& stacks);

		namespace unary {

			const Operands::Type compileArgument(const std::string& symbol, Operands& stacks);

			void scMinus(CalcStacks& stacks);

			void vecMinus(CalcStacks& stacks);

		}

		namespace binary {

			using namespace std::placeholders;
			using oprnd_t = Operands::Type;

			const std::vector<std::string> symbols{
				flexMC::PLUS,
				flexMC::MINUS,
				flexMC::MUL,
				flexMC::DIV,
				flexMC::POW,
			};


			std::string compileArguments(const std::string& symbol, Operands& stacks);

			std::string makeKey(const std::string& symbol, const oprnd_t& left_t, const oprnd_t& right_t);

			void scSc(CalcStacks&, double (*call_back) (const double&, const double&));

			void scVec(CalcStacks&, double (*call_back) (const double&, const double&));

			void vecSc(CalcStacks&, double (*call_back) (const double&, const double&));

			void vecVec(CalcStacks&, double (*call_back) (const double&, const double&));

			const std::unordered_map<std::string, std::function<void(CalcStacks&)>> operators = {
			{
			makeKey(flexMC::MINUS, oprnd_t::scalar, oprnd_t::scalar),
			std::bind(binary::scSc, _1, [](const double& left, const double& right) {return left - right; })
			},
			{
			makeKey(flexMC::MINUS, oprnd_t::scalar, oprnd_t::vector),
			std::bind(binary::scVec, _1, [](const double& left, const double& right) {return left - right; })
			},
			{
			makeKey(flexMC::MINUS, oprnd_t::vector, oprnd_t::scalar),
			std::bind(binary::vecSc, _1, [](const double& left, const double& right) {return left - right; })
			},
			{
			makeKey(flexMC::MINUS, oprnd_t::vector, oprnd_t::vector),
			std::bind(binary::vecVec, _1, [](const double& left, const double& right) {return left - right; })
			},
			{
			makeKey(flexMC::PLUS, oprnd_t::scalar, oprnd_t::scalar),
			std::bind(binary::scSc, _1, [](const double& left, const double& right) {return left + right; })
			},
			{
			makeKey(flexMC::PLUS, oprnd_t::scalar, oprnd_t::vector),
			std::bind(binary::scVec, _1, [](const double& left, const double& right) {return left + right; })
			},
			{
			makeKey(flexMC::PLUS, oprnd_t::vector, oprnd_t::scalar),
			std::bind(binary::vecSc, _1, [](const double& left, const double& right) {return left + right; })
			},
			{
			makeKey(flexMC::PLUS, oprnd_t::vector, oprnd_t::vector),
			std::bind(binary::vecVec, _1, [](const double& left, const double& right) {return left + right; })
			},
			{
			makeKey(flexMC::MUL, oprnd_t::scalar, oprnd_t::scalar),
			std::bind(binary::scSc, _1, [](const double& left, const double& right) {return left * right; })
			},
			{
			makeKey(flexMC::MUL, oprnd_t::scalar, oprnd_t::vector),
			std::bind(binary::scVec, _1, [](const double& left, const double& right) {return left * right; })
			},
			{
			makeKey(flexMC::MUL, oprnd_t::vector, oprnd_t::scalar),
			std::bind(binary::vecSc, _1, [](const double& left, const double& right) {return left * right; })
			},
			{
			makeKey(flexMC::MUL, oprnd_t::vector, oprnd_t::vector),
			std::bind(binary::vecVec, _1, [](const double& left, const double& right) {return left * right; })
			},
			{
			makeKey(flexMC::DIV, oprnd_t::scalar, oprnd_t::scalar),
			std::bind(binary::scSc, _1, [](const double& left, const double& right) {return left / right; })
			},
			{
			makeKey(flexMC::DIV, oprnd_t::scalar, oprnd_t::vector),
			std::bind(binary::scVec, _1, [](const double& left, const double& right) {return left / right; })
			},
			{
			makeKey(flexMC::DIV, oprnd_t::vector, oprnd_t::scalar),
			std::bind(binary::vecSc, _1, [](const double& left, const double& right) {return left / right; })
			},
			{
			makeKey(flexMC::DIV, oprnd_t::vector, oprnd_t::vector),
			std::bind(binary::vecVec, _1, [](const double& left, const double& right) {return left / right; })
			},
			{
			makeKey(flexMC::POW, oprnd_t::scalar, oprnd_t::scalar),
			std::bind(binary::scSc, _1, [](const double& left, const double& right) {return std::pow(left, right); })
			},
			{
			makeKey(flexMC::POW, oprnd_t::scalar, oprnd_t::vector),
			std::bind(binary::scVec, _1, [](const double& left, const double& right) {return std::pow(left, right); })
			},
			{
			makeKey(flexMC::POW, oprnd_t::vector, oprnd_t::scalar),
			std::bind(binary::vecSc, _1, [](const double& left, const double& right) {return std::pow(left, right); })
			},
			{
			makeKey(flexMC::POW, oprnd_t::vector, oprnd_t::vector),
			std::bind(binary::vecVec, _1, [](const double& left, const double& right) {return std::pow(left, right); })
			}
			};

		}

	}

}