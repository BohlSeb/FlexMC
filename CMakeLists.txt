cmake_minimum_required(VERSION 3.28.0)
project(FlexMC VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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