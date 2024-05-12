project(tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCES 
"test_lexer.cpp"
"test_expression_parser.cpp"
"test_expression_compiler.cpp"
)

add_executable(tests ${TEST_SOURCES})

add_dependencies(tests flexmc)

target_link_libraries(tests GTest::gtest_main flexmc)

include(GoogleTest)

add_test(tests tests)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_SOURCES})
