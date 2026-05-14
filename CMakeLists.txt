cmake_minimum_required(VERSION 3.30)

#
# compiler
#
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(cmake_example)

#
# packages
#
find_package(verilator REQUIRED)
find_program(VERYL_EXECUTABLE veryl REQUIRED)

#
# Verilator requires generated SV to exist
# during configure step
#
execute_process(
    COMMAND ${VERYL_EXECUTABLE} build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE VERYL_RESULT
)

if(NOT VERYL_RESULT EQUAL 0)
    message(FATAL_ERROR "veryl build failed")
endif()

#
# generated sv
#
set(GENERATED_SV
    ${CMAKE_CURRENT_SOURCE_DIR}/target/top.sv
)

#
# veryl sources
#
file(GLOB_RECURSE VERYL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.veryl
)

#
# always-check veryl build target
#
add_custom_target(
    veryl_build
    COMMAND ${VERYL_EXECUTABLE} build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${VERYL_SOURCES}
    COMMENT "Running veryl build"
)

#
# simulator
#
add_executable(
    Vtop
    ./test/sim_main.cpp
)

add_dependencies(Vtop veryl_build)

#
# verilator
#
verilate(
    Vtop
    SOURCES ${GENERATED_SV}

    VERILATOR_ARGS
        "--cc"
        "--public"
        "--trace-fst"
)

#
# compile options
#
target_compile_features(Vtop PRIVATE cxx_std_20)

target_compile_options(
    Vtop
    PRIVATE
        -Wall
#        -Wextra
        -O2
)