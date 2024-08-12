project(swig_python VERSION 0.0.1 LANGUAGES CXX)

find_package(Python3 COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})
message("Python.h found: ${Python3_LIBRARIES}")

find_package(SWIG 4.0 COMPONENTS python)
if(SWIG_FOUND)
    message("SWIG found: ${SWIG_EXECUTABLE}")
    if(NOT SWIG_FOUND)
        message(WARNING "SWIG not found")
    endif()
endif()

include (UseSWIG)

# add_subdirectory(src)

set(PY_SWIG_SOURCES
        flexmc.i
)
set_source_files_properties(${PY_SWIG_SOURCES} PROPERTIES CPLUSPLUS ON)

set_property(SOURCE ${PY_SWIG_SOURCES} PROPERTY GENERATED_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

swig_add_library(pyflexmc LANGUAGE python SOURCES ${PY_SWIG_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build"
)

# add_dependencies(pyflexmc flexmc)

target_include_directories(pyflexmc PUBLIC ${PROJECT_BINARY_DIR})

set_target_properties(pyflexmc PROPERTIES SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

target_link_libraries(pyflexmc flexmc flexmc_compiler_flags ${Python3_LIBRARIES})

