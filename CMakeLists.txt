cmake_minimum_required(VERSION 3.28.0)
project(FlexMC VERSION 0.0.1 LANGUAGES CXX)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

add_library(flexmc_compiler_flags INTERFACE)

target_compile_features(flexmc_compiler_flags INTERFACE cxx_std_23)
target_compile_options(flexmc_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_subdirectory(flexmc)
add_subdirectory(app)
add_subdirectory(tests)


# include(FetchContent)
# FetchContent_Declare(
# 	fmt
# 	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
# 	GIT_TAG 10.2.1
# )
# 
# FetchContent_MakeAvailable(fmt)

# add_executable(FlexMC flexmc/flexmc.cpp)
# target_link_libraries(FlexMC PRIVATE fmt::fmt)