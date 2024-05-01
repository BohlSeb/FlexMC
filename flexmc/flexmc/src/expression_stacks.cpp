#include <cassert>
#include "expression_stacks.h"

namespace flexMC {

	void Operands::pushType(const Type& type) {
		assert((type == Operands::Type::scalar) || (type == Operands::Type::date));
		types_.push_back(type);
	}

	void Operands::pushArray(const Type& type, const int& size) {
		assert((type == Operands::Type::vector) || (type == Operands::Type::dateList));
		types_.push_back(type);
		sizes_.push_back(size);
	}

	void Operands::popType() {
		assert(types_.size() >= 0);
		Operands::Type back = types_.back();
		if ((back == Operands::Type::vector) || (back == Operands::Type::dateList)) {
			assert(sizes_.size() >= 0);
			sizes_.pop_back();
		}
		types_.pop_back();
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

	std::ostream& operator<<(std::ostream& output, Operands::Type o_type) {
		using oType = Operands::Type;
		switch (o_type) {
		case oType::scalar:
			output << "scalar";
			break;
		case oType::date:
			output << "date";
			break;
		case oType::vector:
			output << "vector";
			break;
		case oType::dateList:
			output << "dateList";
			break;
		}
		return output;
	}

}