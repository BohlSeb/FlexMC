#pragma once

#include <cassert>
#include <string_view>
#include <vector>
#include <variant>
#include <optional>
#include <utility>

#include "calc_types.h"
#include "language_error.h"
#include "expression_stacks.h"
#include "utils.h"

namespace flexMC
{

    class StaticVCompiler;

    class StaticVStorage
    {

    public:

        bool contains(const std::string_view name) const
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
            constexpr CType c_type = getCType<T>();
            eraseOtherTypeIf<T>(name);
            types_[name] = c_type;
            unused_[name] = c_type;
            std::get<VMap<T>>
                    (maps_.at(c_type)).data[name] = value;
        }

    private:

        friend class StaticVCompiler;

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

        void use(const std::string &name)
        {
            if (unused_.contains(name))
            {
                unused_.erase(name);
            }
        }

        void compileSingleType(Operands &stacks, const std::string &name, const CType &c_type)
        {
            // std::get actual value not checked
            assert((contains(name)) && (cType(name) == c_type));
            use(name);
            stacks.pushType(c_type);
        };

        template<class T>
        T &get(const std::string &name)
        {
            constexpr CType c_type = getCType<T>();
            assert((contains(name)) && (cType(name) == c_type));
            use(name);
            return std::get<VMap<T>>
                    (maps_.at(c_type)).data.at(name);
        }

        template<class T>
        void compileArrayType(Operands &stacks, const std::string &name)
        {
            constexpr CType c_type = getCType<T>();
            static_assert((c_type == CType::vector) || (c_type == CType::dateList));
            const size_t s = get<T>(name).size();
            stacks.pushArray(c_type, s);
        };

        template<class T>
        Operation compile(const std::string &);

    };

    // Declaring/defining template specialized methods within class body raises
    // error: explicit specialization in non-namespace scope 'class flexMC::StaticVStorage',
    // doing it below instead

    template<class T>
    Operation StaticVStorage::compile(const std::string &)
    {
        static_assert(getCType<T>() != CType::undefined, "No template specialisation for the given type T");
        return Operation([](const CalcStacks &)
                         {/* must be overwritten by template specialization */});
    }

    template<>
    Operation StaticVStorage::compile<SCALAR>(const std::string &name)
    {
        const SCALAR value = get<SCALAR>(name);
        return Operation([value](CalcStacks &stacks)
                         { stacks.pushScalar(value); });
    }

    template<>
    Operation StaticVStorage::compile<VECTOR>(const std::string &name)
    {
        const VECTOR value = get<VECTOR>(name);
        return Operation([value](CalcStacks &stacks)
                         { stacks.pushVector(value); });
    }

    template<>
    Operation StaticVStorage::compile<DATE>(const std::string &name)
    {
        const DATE value = get<DATE>(name);
        return Operation([value](CalcStacks &stacks)
                         { stacks.pushDate(value); });
    }

    template<>
    Operation StaticVStorage::compile<DATE_LIST>(const std::string &name)
    {
        const DATE_LIST value = get<DATE_LIST>(name);
        return Operation([value](CalcStacks &stacks)
                         { stacks.pushDateList(value); });
    }

    class StaticVCompiler
    {
    public:

        enum class Status
        {
            found,
            not_found
        };

        // use std::optional
        // coupling of Operands and StaticVStorage?
        static std::pair<Status, std::optional<Operation>>
        tryCompile(const std::string &name, Operands &stacks, StaticVStorage &storage)
        {
            using
            enum CType;
            using
            enum Status;
            if (storage.contains(name))
            {
                const CType c_type = storage.cType(name);
                assert(c_type != undefined);
                switch (c_type)
                {
                    case scalar:
                        storage.compileSingleType(stacks, name, c_type);
                        return {found, storage.compile<SCALAR>(name)};
                    case vector:
                        storage.compileArrayType<VECTOR>(stacks, name);
                        return {found, storage.compile<VECTOR>(name)};
                    case date:
                        storage.compileSingleType(stacks, name, c_type);
                        return {found, storage.compile<DATE>(name)};
                    case dateList:
                        storage.compileArrayType<DATE_LIST>(stacks, name);
                        return {found, storage.compile<DATE_LIST>(name)};
                    default:
                        return {not_found, {}};
                }

            }
            return {not_found, {}};
        }

    };
}
