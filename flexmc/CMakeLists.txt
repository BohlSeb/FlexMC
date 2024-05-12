project(flexmc VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(src)

target_link_libraries(flexmc PRIVATE flexmc_compiler_flags)

target_include_directories(flexmc PRIVATE src)

target_include_directories(flexmc INTERFACE 
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/expression
)