project(flexmc VERSION 0.0.1 LANGUAGES CXX)

# Dependencies
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

add_subdirectory(src)

target_link_libraries(flexmc PRIVATE
        fmt
        flexmc_compiler_flags
)

target_include_directories(flexmc PRIVATE
        src
        src/expression
        src/expression/operations
        src/statement
)

target_include_directories(flexmc INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/expression
        ${CMAKE_CURRENT_SOURCE_DIR}/src/expression/operations
        ${CMAKE_CURRENT_SOURCE_DIR}/src/statement
)