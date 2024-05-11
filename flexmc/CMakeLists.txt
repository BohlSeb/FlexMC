project(flexmc VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(src)

target_include_directories(flexmc PRIVATE src)