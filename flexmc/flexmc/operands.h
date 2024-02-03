 #pragma once

#include <vector>


// Todo: Keep precision in mind

namespace flexMC {

	class Operands {

	public:

		Operands() : typeAsked_(false), size_(0) {}

		void pushValue(const double& value);

		void pushList(const std::vector<double>& values);

		const bool nextIsValue();

		const bool nextIsList();

		const double popValue();

		const std::vector<double> popList();

		const unsigned int size();

	private:

		bool checkType(char type);

		std::vector<double> values_;
		std::vector<std::vector<double>> lists_;
		std::vector<char> types_;

		bool typeAsked_;
		unsigned int size_;

	};

}
