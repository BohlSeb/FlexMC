#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace flexMC {

    using SCALAR = double;
    using VECTOR = std::vector<SCALAR>;
    using DATE = int;
    using DATE_LIST = std::vector<DATE>;

    enum class CType {
        scalar,
        date,
        vector,
        date_list,
        undefined
    };

    struct CTypeHash {

        using is_transparent [[maybe_unused]] = void;

        inline std::size_t operator()(const CType &c) const {
            return static_cast<size_t>(c);
        }

    };

    template<class T>
    using CTypeMap = std::unordered_map<CType, T, CTypeHash, std::equal_to<>>;

    template<class T>
    struct CTypeMapper {
        static constexpr CType value = CType::undefined;
    };

    template<>
    struct CTypeMapper<SCALAR> {
        static constexpr CType value = CType::scalar;
    };

    template<>
    struct CTypeMapper<VECTOR> {
        static constexpr CType value = CType::vector;
    };

    template<>
    struct CTypeMapper<DATE> {
        static constexpr CType value = CType::date;
    };

    template<>
    struct CTypeMapper<DATE_LIST> {
        static constexpr CType value = CType::date_list;
    };

    template<class T>
    constexpr CType getCType() {
        static_assert(CTypeMapper<T>::value != CType::undefined, "Type T does not map to a valid CType");
        return CTypeMapper<T>::value;
    }

    std::string cType2Str(const CType &type);

}
