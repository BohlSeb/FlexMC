#include <cassert>
#include "expression_stacks.h"

namespace flexMC {

	void Operands::pushType(const Type& type) {
		assert((type == Type::scalar) || (type == Type::date));
		if (type == Type::scalar) {
			++scalar_size_;
			scalar_size_max_ = std::max<int>(scalar_size_, scalar_size_max_);
		}
		types_.push_back(type);
	}

	void Operands::pushArray(const Type& type, const int& size) {
		assert((type == Type::vector) || (type == Type::dateList));
		types_.push_back(type);
		vector_sizes_.push_back(size);
		if (type == Type::vector) {
			++vec_size_;
			vec_size_max_ = std::max<int>(vec_size_max_, vec_size_);
		}
	}

	void Operands::popType() {
		assert(types_.size() >= 0);
		Type back = types_.back();
		if ((back == Type::vector) || (back == Type::dateList)) {
			assert(vector_sizes_.size() >= 0);
			vector_sizes_.pop_back();
		}
		switch (back) {
		case Type::scalar: 
			--scalar_size_;
			break;
		case Type::vector: 
			--vec_size_;
			break;
		default: 
			break;
		}
		types_.pop_back();
	}

	const int Operands::maxSize(const Type& type) const {
		switch (type) {
		case Type::scalar: 
			return scalar_size_max_;
		case Type::vector: 
			return vec_size_max_;
		default:
			return 0;
		}

	}

	const std::string Operands::type2Str(const Type& type) {
		switch (type)
		{
		case Type::scalar: { return "Scalar"; }
		case Type::vector: { return "Vector"; }
		case Type::date: { return "Date"; }
		case Type::dateList: { return "DateList"; }
		default: { return ""; }
		}
	}

	CalcStacks::CalcStacks(const int& s_size, const int& v_size, const int& d_size, const int& d_l_size) {
		scalars_.reserve(s_size);
		vectors_.reserve(v_size);
		dates_.reserve(d_size);
		date_lists_.reserve(d_l_size);
	}
	

	const std::size_t CalcStacks::size(Operands::Type type) const {
		switch (type) {
		case Operands::Type::scalar:
			return scalars_.size();
		case Operands::Type::date:
			return dates_.size();
		case Operands::Type::vector:
			return vectors_.size();
		case Operands::Type::dateList:
			return date_lists_.size();
		default:
			return 0;
		}
	}

	const bool CalcStacks::ready() const {
		std::size_t not_ready = 0;
		not_ready += size(Operands::Type::scalar);
		not_ready += size(Operands::Type::vector);
		not_ready += size(Operands::Type::date);
		not_ready += size(Operands::Type::dateList);
		return (not_ready == 0);
	}

}