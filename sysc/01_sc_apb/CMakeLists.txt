cmake_minimum_required (VERSION 3.8)

project(apb_example CXX)

set(target run.x)
set(rtl_list
  ./rtl/dut_top.v
  ./rtl/clk_gate.v
  ./rtl/dut_reg.v
)

find_package(verilator HINTS $ENV{VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
# Find SystemC using SystemC's CMake integration
find_package(SystemCLanguage QUIET)

add_executable(${target} sc_main.cpp ./tb/apb_req.cpp)

verilate(${target} SYSTEMC COVERAGE TRACE
  INCLUDE_DIRS "./rtl"
  SOURCES ${rtl_list}
)

verilator_link_systemc(${target})

target_include_directories(${target} PUBLIC "./tb")

