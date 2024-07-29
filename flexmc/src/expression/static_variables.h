#pragma once

#include <cassert>
#include <vector>
#include <variant>

#include "calc_types.h"
#include "utils.h"

namespace flexMC
{

    class StaticVStorage
    {

    public:

        bool contains(const std::string &name) const
        { return types_.contains(name); }

        CType cType(const std::string &name) const
        { return types_.at(name); }

        bool containsUnused() const
        { return unused_.begin() != unused_.end(); }

        StringMap<CType> unused() const
        { return unused_; }

        template<class T>
        void insert(const std::string &name, const T &value)
        {
            const CType c_type = getCType<T>();
            eraseOtherTypeIf<T>(name);
            types_[name] = c_type;
            unused_[name] = c_type;
            std::get<VMap<T>>
                    (maps_.at(c_type)).data[name] = value;
        }

        template<class T>
        T &get(const std::string &name)
        {
            const CType c_type = getCType<T>();
            use(name);
            return std::get<VMap<T>>
                    (maps_.at(c_type)).data.at(name);
        }

    private:

        template<class T>
        struct VMap
        {
            StringMap<T> data;
        };

        using VMapT = std::variant<VMap<SCALAR>, VMap<VECTOR>, VMap<DATE>, VMap<DATE_LIST>>;

        CTypeMap<VMapT> maps_ = initialize();

        StringMap<CType> types_;

        StringMap<CType> unused_;

        CTypeMap<VMapT> initialize() const
        {
            VMap<SCALAR> scalars;
            VMap<VECTOR> vectors;
            VMap<DATE> dates;
            VMap<DATE_LIST> date_lists;
            VMapT scalars_v = scalars;
            VMapT vectors_v = vectors;
            VMapT dates_v = dates;
            VMapT date_lists_v = date_lists;
            CTypeMap<VMapT> maps;
            using
            enum CType;
            maps[scalar] = scalars_v;
            maps[vector] = vectors_v;
            maps[date] = dates_v;
            maps[dateList] = date_lists_v;
            return maps;
        }

        void use(const std::string &name)
        {
            if (unused_.contains(name))
            {
                unused_.erase(name);
            }
        }

        template<class T>
        void eraseOtherTypeIf(const std::string &name)
        {
            if (types_.contains(name))
            {
                const CType c_type = types_.at(name);
                if (c_type != getCType<T>())
                {
                    std::visit(
                            [name](auto &map)
                            {
                                if (map.data.contains(name))
                                { map.data.erase(name); }
                            },
                            maps_.at(c_type));

                    use(name);
                    types_.erase(name);
                }
            }
        }

    };


}
