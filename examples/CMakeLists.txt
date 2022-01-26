# set cmake and compiler options
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(tess_example)

# find the SFML library
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# compilation target for simple example
add_executable(simple src/simple.cpp)
target_link_libraries(simple sfml-graphics sfml-window sfml-system)

# compilation target for highlight example
add_executable(highlight src/highlight.cpp)
target_link_libraries(highlight sfml-graphics sfml-window sfml-system)
