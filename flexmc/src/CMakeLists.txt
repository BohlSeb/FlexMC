set(SOURCES
        terminals.h
        tokens.cpp
        lexer.cpp
        language_error.cpp
        expression/expression_parser.cpp
        expression/expression_compiler.cpp
        expression/expression_stacks.cpp
        expression/operand.cpp
        expression/operations/operation.cpp
        expression/operations/operation_compiler.cpp
        expression/operations/functions_real.cpp
        expression/operations/operators_calc.cpp
        utils.h
)

add_library(flexmc STATIC ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})