#include <gtest/gtest.h>
#include <ranges>

#include "static_variables.h"
#include "expression_compiler.h"


TEST(StaticVStorage, SetGetUnused) {

    using namespace flexMC;

    using
    enum CType;

    SCALAR s{1.0};
    VECTOR v{2.0, 3.0};
    DATE d{4};
    DATE_LIST d_l{1, 0};

    std::string s_name{"s"};
    std::string v_name{"v"};
    std::string d_name{"d"};
    std::string d_l_name{"d_l"};
    std::string s_name_unused{"s_u"};
    std::string v_name_unused{"v_u"};

    StaticVStorage storage;

    storage.insert<SCALAR>(s_name, s);
    storage.insert<SCALAR>(s_name, s);
    storage.insert<SCALAR>(v_name, s);
    storage.insert<VECTOR>(v_name, v);
    storage.insert<DATE>(d_name, d);
    storage.insert<DATE_LIST>(d_l_name, d_l);
    storage.insert<SCALAR>(s_name_unused, s);
    storage.insert<VECTOR>(v_name_unused, v);

    ASSERT_EQ(scalar, storage.cType(s_name));
    ASSERT_EQ(vector, storage.cType(v_name));
    ASSERT_EQ(date, storage.cType(d_name));
    ASSERT_EQ(date_list, storage.cType(d_l_name));

    ASSERT_EQ(6, storage.unused().size());

    const std::vector<std::string> names({s_name, v_name, d_name, d_l_name, d_l_name, d_name, v_name, s_name});
    const std::vector<CType> expected_types({scalar, vector, date, date_list, date_list, date, vector, scalar});

    for (const auto [name, exp]: std::ranges::zip_view(names, expected_types)) {

        Operands operands;
        const auto [status, maybe_operation] = StaticVCompiler::tryCompile(name, operands, storage);
        ASSERT_TRUE(status == StaticVCompiler::Status::found);
        ASSERT_TRUE(operands.haveCompiled());
        const CType return_type = operands.typesBack();
        ASSERT_EQ(return_type, exp);
        CalcStacks calc_stacks{0, 0, 0, 0};
        Expression expression;
        expression.push_back(maybe_operation.value());
        expression(calc_stacks);
        switch (return_type) {
            case scalar:
                ASSERT_EQ(s, calc_stacks.scalars().back());
                calc_stacks.scalars().pop_back();
                break;
            case vector:
                ASSERT_EQ(v, calc_stacks.vectorResult());
                calc_stacks.popVectorResult();
                break;
            case date:
                ASSERT_EQ(d, calc_stacks.dates().back());
                calc_stacks.dates().pop_back();
                break;
            case date_list:
                ASSERT_EQ(d_l, calc_stacks.dateListResult());
                calc_stacks.popDateListResult();
                break;
            default:
                break;

        }
        ASSERT_TRUE(calc_stacks.ready());
    }

    ASSERT_EQ(2, storage.unused().size());

    Operands operands;
    [[maybe_unused]] auto [status_s, operation_s] = StaticVCompiler::tryCompile(s_name_unused, operands, storage);

    ASSERT_EQ(v_name_unused, std::get<0>(*(storage.unused().begin())));
    ASSERT_EQ(vector, std::get<1>(*(storage.unused().begin())));

    [[maybe_unused]] auto [status_v, operation_v] = StaticVCompiler::tryCompile(v_name_unused, operands, storage);
    ASSERT_EQ(0, storage.unused().size());

    storage.insert<SCALAR>(v_name, s);
    storage.insert<VECTOR>(d_name, v);
    storage.insert<DATE>(d_l_name, d);
    storage.insert<DATE_LIST>(s_name, d_l);

    ASSERT_EQ(4, storage.unused().size());

}
