# code adapted from
#   https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-
#   with-sphinx-breathe-doxygen-cmake/
# and from
#   https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

# set cmake and compiler options
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project("hax")

# include custom modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# define project structure variables
set(SOURCE_EXT "cpp")
set(HEADER_EXT "hpp")
set(HAX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(HAX_PUBLIC_HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB HAX_SOURCES ${HAX_SOURCE_DIR}/*.${SOURCE_EXT})
file(GLOB HAX_PUBLIC_HEADERS ${HAX_PUBLIC_HEADER_DIR}/*.${HEADER_EXT})

# include the documentation
add_subdirectory(${PROJECT_SOURCE_DIR}/docs)

# setup the library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
add_library(hax ${HAX_SOURCES})
target_include_directories(hax PUBLIC ${HAX_PUBLIC_HEADER_DIR})
