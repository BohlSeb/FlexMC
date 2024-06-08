set(SOURCES
        "terminals.h"
        "tokens.cpp"
        "lexer.cpp"
        "expression/expression_parser.cpp"
        "expression/expression_compiler.cpp"
        "expression/expression_stacks.cpp"
        "expression/functions_real.cpp"
        "expression/operand.cpp"
        "expression/operation.cpp"
        "expression/operation_compiler.cpp"
        "expression/operators_calc.cpp"
        language_error.h
        language_error.cpp
)

add_library(flexmc STATIC ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})