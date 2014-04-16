CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(_VERSION_MAJOR 0)
SET(_VERSION_MINOR 1)
SET(_AUTHORS "\"Howard Grimberg, Pho Hale, Seth Polsley, and Peter Lesslie\"")
SET(_PROJECT_NAME "\"QSolve\"")
PROJECT(qsolve)

SET(DEBUG_FLAGS "-fsanitizer=address -fno-omit-frame-pointer")
SET(CXX11_FLAGS "--std=c++11 -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

#find and link to python
FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

# find and link to boost
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.40 COMPONENTS python thread program_options date_time REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

# configure a header file to pass some CMake settings
# to the source code
CONFIGURE_FILE(
  "${qsolve_SOURCE_DIR}/Config.h.in"
  "${qsolve_BINARY_DIR}/Config.h"
)

# copy PyDatabase file
CONFIGURE_FILE(
  "${qsolve_SOURCE_DIR}/PyDatabase.py"
  "${qsolve_BINARY_DIR}/PyDatabase.py"
  COPY_ONLY
)

# copy PyStrategy file
# just for testing purposes
CONFIGURE_FILE(
  "${qsolve_SOURCE_DIR}/PyStrategy.py"
  "${qsolve_BINARY_DIR}/PyStrategy.py"
  COPY_ONLY
)

# copy table.csv file
# for MockDatabase class
CONFIGURE_FILE(
  "${qsolve_SOURCE_DIR}/table.csv"
  "${qsolve_BINARY_DIR}/table.csv"
  COPY_ONLY
)

INCLUDE_DIRECTORIES("${qsolve_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${qsolve_BINARY_DIR}")

ADD_LIBRARY( Context Context.cpp OrderFactory.cpp )

# add executable
#PYTHON_ADD_MODULE(qsolve_ext PyContext.cpp)
#TARGET_LINK_LIBRARIES(qsolve_ext Context)
#TARGET_LINK_LIBRARIES(qsolve_ext ${Boost_Libraries})

ADD_EXECUTABLE(qsolve main.cpp)
TARGET_LINK_LIBRARIES(qsolve Context)
TARGET_LINK_LIBRARIES(qsolve boost_program_options ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
