cmake_minimum_required (VERSION 2.6)
set (_VERSION_MAJOR 0)
set (_VERSION_MINOR 1)
set (_AUTHORS "\"Howard Grimberg, Pho Hale, Seth Polsley, and Peter Lesslie\"")
project (Qsolve)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# find and link to boost
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# configure a header file to pass some CMake settings
# to the source code
configure_file (
  "${Qsolve_SOURCE_DIR}/Config.h.in"
  "${Qsolve_BINARY_DIR}/Config.h"
)

include_directories("${Qsolve_SOURCE_DIR}" "${Qsolve_SOURCE_DIR}/Database" "${Qsolve_BINARY_DIR}" "${Qsolve_SOURCE_DIR}/OrderEngine" "${Qsolve_SOURCE_DIR}/Portfolio")
add_subdirectory(Database)
add_subdirectory(OrderEngine)
add_subdirectory(Portfolio)

# add executable
add_executable (Qsolve main.cpp)
target_link_libraries(Qsolve Database)
target_link_libraries(Qsolve OrderEngine)
target_link_libraries(Qsolve Portfolio)
target_link_libraries(Qsolve boost_program_options)

