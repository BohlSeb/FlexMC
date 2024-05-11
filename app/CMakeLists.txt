project(app VERSION 0.0.1 LANGUAGES CXX)

configure_file(app_config.h.in app_config.h)

add_subdirectory(src)

add_dependencies(app flexmc)

target_link_libraries(app flexmc)

target_include_directories(app PUBLIC 
${PROJECT_BINARY_DIR} 
${CMAKE_SOURCE_DIR}/flexmc/src 
${CMAKE_SOURCE_DIR}/flexmc/src/expression
)



