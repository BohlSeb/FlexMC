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

add_subdirectory(src)

add_dependencies(tests flexmc)

target_link_libraries(tests GTest::gtest_main flexmc)

target_include_directories(tests PUBLIC 
${CMAKE_SOURCE_DIR}/flexmc/src 
${CMAKE_SOURCE_DIR}/flexmc/src/expression
)

include(GoogleTest)

add_test(tests tests)






#set(UNIT_TEST_TARGET_NAME test_suite)

#add_library(${UNIT_TEST_TARGET_NAME} SHARED)

#target_sources(${UNIT_TEST_TARGET_NAME} PRIVATE 
#    src/test_lexer.cpp
#    # tests.h
#)

#add_dependencies(app flexmc)

#target_link_libraries(${UNIT_TEST_TARGET_NAME} PRIVATE 
#MSUnitTestFramework::MSUnitTestFramework
#flexmc
#)

#target_include_directories(test_suite PUBLIC 
#${CMAKE_SOURCE_DIR}/flexmc/src 
#${CMAKE_SOURCE_DIR}/flexmc/src/expression
#)

#enable_testing()

#find_program(VSTest_EXECUTABLE NAME vstest.console.exe REQUIRED)

#list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#find_package(MSUnitTestFramework REQUIRED)

#add_test(NAME ${UNIT_TEST_TARGET_NAME} 
#    COMMAND "${VSTest_EXECUTABLE}" "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${UNIT_TEST_TARGET_NAME}.dll"
#)