project(benchmarking VERSION 0.0.1 LANGUAGES CXX)

# Dependencies
include(FetchContent)
FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.5
)

FetchContent_MakeAvailable(benchmark)

add_subdirectory(src)

add_dependencies(benchmarking flexmc)

target_link_libraries(benchmarking benchmark flexmc flexmc_compiler_flags)

target_include_directories(benchmarking PUBLIC ${PROJECT_BINARY_DIR})