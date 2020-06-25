# set cmake and compiler options
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(hax_example)

# find the SFML library
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# set up the hax library
set(HAX_HEADER_DIR ${PROJECT_SOURCE_DIR}/../include)
add_library(hax INTERFACE)
target_include_directories(hax INTERFACE ${HAX_HEADER_DIR})

set(EXAMPLE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# compilation target for simple example
add_executable(simple ${EXAMPLE_SOURCE_DIR}/simple.cpp)
target_link_libraries(simple sfml-graphics sfml-window sfml-system hax)

# compilation target for highlight example
add_executable(highlight ${EXAMPLE_SOURCE_DIR}/highlight.cpp)
target_link_libraries(highlight sfml-graphics sfml-window sfml-system hax)
