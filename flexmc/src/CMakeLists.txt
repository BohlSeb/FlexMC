set(SOURCES
        terminals.h
        utils.h
        tokens.cpp
        lexer.cpp
        calc_types.cpp
        language_error.cpp
        expression/static_variables.h
        expression/expression_parser.cpp
        expression/expression_compiler.cpp
        expression/expression_stacks.cpp
        expression/operand.cpp
        expression/operations/operation_compiler.cpp
        expression/operations/functions_real.cpp
        expression/operations/operators_calc.cpp
        parser/statement_definitions.h
        parser/line_parser.h
        parser/line_parser.cpp
)

add_library(flexmc STATIC ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})