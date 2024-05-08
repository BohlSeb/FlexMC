#pragma once
#include <iostream>
#include <vector>


namespace flexMC {

	class Operands {

	public:

		enum class Type {
			scalar,
			date,
			vector,
			dateList
		};

		struct CompileReport {
			CompileReport(Type ret_type, const int max_scalar, const int max_vector) :
				ret_type(ret_type), max_scalar(max_scalar), max_vector(max_vector) {}
			const Type ret_type;
			const int max_scalar;
			const int max_vector;
		};

		const bool haveCompiled() const { return ((types_.size() == 1) && (function_symbols_.size() == 0)); }

		const CompileReport report() const { return CompileReport(types_.back(), scalar_size_max_, vec_size_max_); }

		void pushType(const Type& type);

		void popType();

		void pushArray(const Type& type, const int& size);

		const Type typesBack() const { return types_.back(); }

		const int sizesBack() const { return vector_sizes_.back(); }

		void pushFunc(const std::string& symbol) { function_symbols_.push_back(symbol); }

		const std::string funcsBack() const { return function_symbols_.back(); }

		void popFunc() { function_symbols_.pop_back(); }

		const std::size_t tSize() const { return types_.size(); }

		const std::size_t fSize() const { return function_symbols_.size(); }

		const int maxSize(const Type& type) const;

	private:

		std::vector<Type> types_;

		std::vector<std::string> function_symbols_;

		std::vector<int> vector_sizes_;

		int vec_size_ = 0;
		int vec_size_max_ = 0;
		int scalar_size_ = 0;
		int scalar_size_max_ = 0;

	};

	class CalcStacks {

	public:

		CalcStacks(const int& s_size, const int& v_size, const int& d_size, const int& d_l_size);

		void pushScalar(const double& value) { scalars_.push_back(value); }

		void pushVector(const std::vector<double>& value) { vectors_.push_back(value); }

		void pushDate(const int& value) { dates_.push_back(value); }

		void pushDateList(const std::vector<int>& value) { date_lists_.push_back(value); }

		const double& scalarsBack() const { return scalars_.back(); }

		std::vector<double>& vectorsBack() { return vectors_.back(); }

		std::vector<double>& vectorsBeforeBack() { return vectors_.end()[-2]; }

		const int& datesBack() const { return dates_.back(); }

		std::vector<int>& dateListsBack() { return date_lists_.back(); }

		void popScalar() { scalars_.pop_back(); }

		void popVector() { vectors_.pop_back(); }

		void popDate() { dates_.pop_back(); };

		void popDateList() { date_lists_.pop_back(); }

		const bool ready() const;

		std::vector<double>::const_iterator scalarsStart() { return scalars_.cbegin(); }

		std::vector<double>::const_iterator scalarsEnd() { return scalars_.cend(); }

		//const bool hasCalculated() const;

		//const bool hasReturnType(const Operands::Type& expected) const;

		// for debugging?
		const std::size_t size(Operands::Type type) const;

	private:

		std::vector<double> scalars_;

		std::vector<std::vector<double>> vectors_;

		std::vector<int> dates_;

		std::vector<std::vector<int>> date_lists_;

	};

	class PostFixItem {

	public:

		virtual void evaluate(CalcStacks& stacks) {}

	};

	std::ostream& operator<<(std::ostream& output, Operands::Type o_type);

}