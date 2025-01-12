#include "calc_types.h"

namespace flexMC {

    std::string cType2Str(const CType &type) {
        using
        enum CType;
        switch (type) {
            case scalar:
                return "SCALAR";
            case vector:
                return "LIST_OF_SCALAR";
            case date:
                return "DATE";
            case date_list:
                return "LIST_OF_DATES";
            case undefined:
                return "UNDEFINED";
            default:
                return "";
        }
    }

}
