cmake_minimum_required (VERSION 2.6)
set (_VERSION_MAJOR 0)
set (_VERSION_MINOR 1)
project (cppqsolve)

# find and link to boost
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# configure a header file to pass some CMake settings
# to the source code
configure_file (
  "${cppqsolve_SOURCE_DIR}/Config.h.in"
  "${cppqsolve_BINARY_DIR}/Config.h"
)

include_directories("${cppqsolve_SOURCE_DIR}" "${cppqsolve_SOURCE_DIR}/Database" "${cppqsolve_BINARY_DIR}" "${cppqsolve_SOURCE_DIR}/OrderEngine")
add_subdirectory(Database)
add_subdirectory(OrderEngine)

# add executable
add_executable (cppqsolve main.cpp)
target_link_libraries(cppqsolve Database)
target_link_libraries(cppqsolve OrderEngine)
