cmake_minimum_required (VERSION 2.6)
set (_VERSION_MAJOR 0)
set (_VERSION_MINOR 1)
project (cppqsolve)

MESSAGE("project SOURCE_DIR is ${cppqsolve_SOURCE_DIR}")

# configure a header file to pass some CMake settings
# to the source code
configure_file (
  "${cppqsolve_SOURCE_DIR}/Config.h.in"
  "${cppqsolve_BINARY_DIR}/Config.h"
)

include_directories("${cppqsolve_BINARY_DIR}")

# add executable
add_executable (cppqsolve main.cpp)
