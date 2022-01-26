# set cmake and compiler options
cmake_minimum_required(VERSION 3.10)
project(tess)

# variables for install destinations
include(GNUInstallDirs)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# specify the file interface
set(tess_SRCEXT cpp)
set(tess_INCEXT hpp)
set(tess_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)

# Add the source code
add_subdirectory(src)

# Add an option to compile the documentation
option(DOCS "Compile documentation" OFF)
if (DOCS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/docs)
endif()

# Add an option to compile tests
option(TESTING "Compile tests" OFF)
if (TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()
