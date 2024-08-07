cmake_minimum_required(VERSION 3.28.0)
project(FlexMC VERSION 0.0.1 LANGUAGES CXX)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

option(LINK_ASAN "add link options regarding address sanitizer" OFF)

add_library(flexmc_compiler_flags INTERFACE)

target_compile_features(flexmc_compiler_flags INTERFACE cxx_std_23)
target_compile_options(flexmc_compiler_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        # "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wformat=2>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

if (LINK_ASAN)
    add_link_options(-fsanitize=address)
    target_link_options(flexmc_compiler_flags INTERFACE -fsanitize=address)
    message("linking ASAN")
else ()
    message("skipping ASAN")
endif()


add_subdirectory(flexmc)
add_subdirectory(app)
add_subdirectory(tests)
if (LINK_ASAN)
    message("skipping benchmark in ASAN build")
else ()
    message("adding benchmark")
    add_subdirectory(benchmarking)
endif()
