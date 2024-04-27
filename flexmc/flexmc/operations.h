#pragma once

#include <vector>
#include <cmath>
#include <functional>
#include <algorithm>


namespace flexMC {

	namespace binary {

		const double plus(const double& left, const double& right) { return left + right; }

		const std::vector<double> plus(const std::vector<double>& left, const double& right) {
			std::vector<double> res;
			std::transform(left.cbegin(), left.cend(), res.begin(), [&right](const double& l) {return l + right; });
			return res;
		}

		const std::vector<double> plus(const double& left, const std::vector<double>& right) {
			std::vector<double> res;
			std::transform(right.cbegin(), right.cend(), res.begin(), [&left](const double& r) {return left + r; });
			return res;
		}

		const std::vector<double> plus(const std::vector<double>& left, const std::vector<double>& right) {
			std::vector<double> res;
			std::transform(left.cbegin(), left.cend(), right.cbegin(), res.begin(), std::plus<double>());
			return res;
		}

		const double minus(const double& left, const double& right) { return left - right; }

		const std::vector<double> minus(const std::vector<double>& left, const double& right) {
			std::vector<double> res;
			std::transform(left.cbegin(), left.cend(), res.begin(), [&right](const double& l) {return l - right; });
			return res;
		}

		const std::vector<double> minus(const double& left, const std::vector<double>& right) {
			std::vector<double> res;
			std::transform(right.cbegin(), right.cend(), res.begin(), [&left](const double& r) {return left - r; });
			return res;
		}

		const std::vector<double> minus(const std::vector<double>& left, const std::vector<double>& right) {
			std::vector<double> res;
			std::transform(left.cbegin(), left.cend(), right.cbegin(), res.begin(), std::minus<double>());
			return res;
		}


		std::function<std::vector<double>(const std::vector<double>&, const double&)> vecScalar(
			const std::vector<double>& left,
			const double& right,
			double (*func)(const double&, const double&)) {
			std::vector<double> res;
			std::transform(left.cbegin(), left.cend(), res.begin(), [=](const double& l) {return func(l, right); });
			return res;
		}

		inline std::vector<double> plus(const std::vector<double>& left, const double& right) {
			return vecScalar(left, right, plus);
			//std::vector<double> res;
			//res.reserve(left.size());
			//for (auto it = std::begin(left); it != std::end(left); ++it) {
			//	res.emplace_back(*it + right);
			//}
			//return res;
		}

		inline std::vector<double> plus(const double& left, const std::vector<double>& right) {
			return plus(right, left);
		}

		inline std::vector<double> plus(const std::vector<double>& left, const std::vector<double>& right) {
			std::vector<double>::size_type size = left.size();
			std::vector<double> res;
			res.reserve(size);
			for (unsigned int i = 0; i < size; i++) {
				res.emplace_back(left[ i ] + right[ i ]);
			}
			return res;
		}

		inline double minus(const double& left, const double& right) {
			return left - right;
		}

		inline std::vector<double> minus(const std::vector<double>& left, const double& right) {
			std::vector<double> res;
			res.reserve(left.size());
			for (auto it = std::begin(left); it != std::end(left); ++it) {
				res.emplace_back(*it - right);
			}
			return res;
		}

		std::vector<double> minus(const double& left, const std::vector<double>& right) {}

		std::vector<double> minus(const std::vector<double>& left, const std::vector<double>& right);

		inline double mul(const double& left, const double& right) {
			return left * right;
		}

		std::vector<double> mul(const std::vector<double>& left, const double& right);

		std::vector<double> mul(const double& left, const std::vector<double>& right);

		std::vector<double> mul(const std::vector<double>& left, const std::vector<double>& right);

		inline double div(const double& left, const double& right) {
			return left / right;
		}

		std::vector<double> div(const std::vector<double>& left, const double& right);

		std::vector<double> div(const std::vector<double>& left, const std::vector<double>& right);

		inline double pow(const double& left, const double& right) {
			return std::pow(left, right);
		}

		std::vector<double> pow(const std::vector<double>& left, const double& right);

	}

	namespace binaryComp {

		inline double lt(const double& left, const double& right) {
			return (left < right ? 1.0 : 0.0);
		}

		inline double gt(const double& left, const double& right) {
			return (left > right ? 1.0 : 0.0);
		}

		inline double le(const double& left, const double& right) {
			return (left <= right ? 1.0 : 0.0);
		}

		inline double ge(const double& left, const double& right) {
			return (left >= right ? 1.0 : 0.0);
		}

		inline double and_(const double& left, const double& right) {
			return (left >= right ? left : right);
		}

		inline double or_(const double& left, const double& right) {
			return (left >= right ? right : left);
		}

	}

}