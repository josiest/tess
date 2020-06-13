# code adapted from
#   https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-
#   with-sphinx-breathe-doxygen-cmake/
# and from
#   https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

# set cmake and compiler options
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(hax)

# include custom modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# global project structure
set(HAX_HEADER_DIR ${PROJECT_SOURCE_DIR}/include)

# include the documentation
add_subdirectory(${PROJECT_SOURCE_DIR}/docs)

# setup the library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
add_library(hax INTERFACE)
target_include_directories(hax INTERFACE ${HAX_HEADER_DIR})
#set(HAX_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)

# include testing
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
