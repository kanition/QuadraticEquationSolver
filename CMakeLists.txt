cmake_minimum_required(VERSION 3.12)
project("QuadraticEquationSolver")

# hot reload for MSVC
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 20)
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(demo "demo.cpp")

include(CTest)

add_executable(double_test "test/double_test.cpp")
add_test(NAME double_test COMMAND double_test)

add_executable(float_test "test/float_test.cpp")
add_test(NAME float_test COMMAND float_test)

if(MSVC)
  target_compile_options(double_test PRIVATE /source-charset:utf-8)
  target_compile_options(float_test PRIVATE /source-charset:utf-8)
endif()
