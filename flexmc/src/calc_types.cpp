#include "calc_types.h"

namespace flexMC
{

    std::string cType2Str(const CType &type)
    {
        using
        enum CType;
        switch (type)
        {
            case scalar:
                return "Scalar";
            case vector:
                return "Vector";
            case date:
                return "Date";
            case dateList:
                return "DateList";
            case undefined:
                return "undefined";
            default:
                return "";
        }
    }

}
