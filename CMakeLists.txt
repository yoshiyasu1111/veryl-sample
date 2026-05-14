cmake_minimum_required(VERSION 3.30)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(cmake_example)

find_package(verilator HINTS $ENV{VERIOATOR_ROOT})
add_executable(Vtop ./test/sim_main.cpp)
verilate(
  Vtop
  SOURCES ./target/top.sv
  INCLUDE_DIRS ./obj_dir
  VERILATOR_ARGS
    "--public"
    "--trace-fst"
)
