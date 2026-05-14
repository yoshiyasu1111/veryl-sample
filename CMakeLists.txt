cmake_minimum_required(VERSION 4.3)
project(cmake_example)

find_package(verilator HINTS $ENV{VERIOATOR_ROOT})
add_executable(Vtop ./test/sim_main.cpp)
verilate(
  Vtop
  SOURCES ./target/top.sv
  INCLUDE_DIRS ./obj_dir
  VERILATOR_ARGS
    "--public"
)
