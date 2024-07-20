#include <gtest/gtest.h>

#include "static_variables.h"


TEST(StaticVStorage, SetGetUnused)
{
    using namespace flexMC;

    SCALAR s{1.0};
    VECTOR v{1.0, 2.0};
    DATE d{1};
    DATE_LIST d_l{1, 2};

    std::string s_name{"s"};
    std::string v_name{"v"};
    std::string d_name{"d"};
    std::string d_l_name{"d_l"};
    std::string s_name_unused{"s_u"};
    std::string v_name_unused{"v_u"};

    StaticVStorage storage;

    storage.insert<SCALAR>(s_name, s);
    storage.insert<VECTOR>(v_name, v);
    storage.insert<DATE>(d_name, d);
    storage.insert<DATE_LIST>(d_l_name, d_l);
    storage.insert<SCALAR>(s_name_unused, s);
    storage.insert<VECTOR>(v_name_unused, v);

    ASSERT_EQ(CType::scalar, storage.cType(s_name));
    ASSERT_EQ(CType::vector, storage.cType(v_name));
    ASSERT_EQ(CType::date, storage.cType(d_name));
    ASSERT_EQ(CType::dateList, storage.cType(d_l_name));

    ASSERT_EQ(6, storage.unused().size());

    [[maybe_unused]] SCALAR s_get = storage.get<SCALAR>(s_name);
    [[maybe_unused]] VECTOR v_get = storage.get<VECTOR>(v_name);
    [[maybe_unused]] DATE d_get = storage.get<DATE>(d_name);
    [[maybe_unused]] DATE_LIST d_l_get = storage.get<DATE_LIST>(d_l_name);

    ASSERT_EQ(2, storage.unused().size());

    [[maybe_unused]] SCALAR unused_s = storage.get<SCALAR>(s_name_unused);

    ASSERT_EQ(v_name_unused, std::get<0>(*(storage.unused().begin())));
    ASSERT_EQ(CType::vector, std::get<1>(*(storage.unused().begin())));
    ASSERT_EQ(v, storage.get<VECTOR>(v_name_unused));
    ASSERT_EQ(0, storage.unused().size());
    ASSERT_EQ(0, storage.unused().size());

    storage.insert<SCALAR>(v_name, s);
    storage.insert<VECTOR>(d_name, v);
    storage.insert<DATE>(d_l_name, d);
    storage.insert<DATE_LIST>(s_name, d_l);

    ASSERT_EQ(4, storage.unused().size());

    ASSERT_EQ(CType::scalar, storage.cType(v_name));
    ASSERT_EQ(CType::vector, storage.cType(d_name));
    ASSERT_EQ(CType::date, storage.cType(d_l_name));
    ASSERT_EQ(CType::dateList, storage.cType(s_name));

    [[maybe_unused]] SCALAR s_get_ = storage.get<SCALAR>(v_name);
    [[maybe_unused]] VECTOR v_get_ = storage.get<VECTOR>(d_name);
    [[maybe_unused]] DATE d_get_ = storage.get<DATE>(d_l_name);
    [[maybe_unused]] DATE_LIST d_l_get_ = storage.get<DATE_LIST>(s_name);

    ASSERT_EQ(0, storage.unused().size());
}
