cmake_minimum_required (VERSION 2.6)
set (_VERSION_MAJOR 0)
set (_VERSION_MINOR 1)
set (_AUTHORS "\"Howard Grimberg, Pho Hale, Seth Polsley, and Peter Lesslie\"")
set (_PROJECT_NAME "\"QSolve\"")
project (qsolve)

SET(CXX11_FLAGS "-std=c++11")
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}" )

#find and link to python
FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# find and link to boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE( Boost 1.40 COMPONENTS thread program_options date_time REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# configure a header file to pass some CMake settings
# to the source code
configure_file (
  "${qsolve_SOURCE_DIR}/Config.h.in"
  "${qsolve_BINARY_DIR}/Config.h"
)

# copy PyDatabase file
configure_file (
  "${qsolve_SOURCE_DIR}/PyDatabase.py"
  "${qsolve_BINARY_DIR}/PyDatabase.py"
  COPY_ONLY
)

# copy PyStrategy file
# just for testing purposes
configure_file (
  "${qsolve_SOURCE_DIR}/PyStrategy.py"
  "${qsolve_BINARY_DIR}/PyStrategy.py"
  COPY_ONLY
)

# copy table.csv file
# for MockDatabase class
configure_file (
  "${qsolve_SOURCE_DIR}/Database/table.csv"
  "${qsolve_BINARY_DIR}/table.csv"
  COPY_ONLY
)

include_directories("${qsolve_SOURCE_DIR}")
include_directories("${qsolve_BINARY_DIR}")
include_directories("${qsolve_SOURCE_DIR}/Database")
include_directories("${qsolve_SOURCE_DIR}/OrderFactory")
include_directories("${qsolve_SOURCE_DIR}/OrderEngine")
include_directories("${qsolve_SOURCE_DIR}/Portfolio")
include_directories("${qsolve_SOURCE_DIR}/PythonInterpreter")
include_directories("${qsolve_SOURCE_DIR}/StrategyEvaluator")

add_subdirectory(OrderEngine)
add_subdirectory(Database)
add_subdirectory(OrderFactory)
add_subdirectory(Portfolio)
add_subdirectory(PythonInterpreter)
add_subdirectory(StrategyEvaluator)

# add executable
add_executable (qsolve main.cpp)
target_link_libraries(qsolve Database)
target_link_libraries(qsolve OrderEngine)
target_link_libraries(qsolve OrderFactory)
target_link_libraries(qsolve Portfolio)
target_link_libraries(qsolve PythonInterpreter)
target_link_libraries(qsolve StrategyEvaluator)
target_link_libraries(qsolve boost_program_options)
target_link_libraries(qsolve ${PYTHON_LIBRARIES})
target_link_libraries(qsolve ${Boost_LIBRARIES})

