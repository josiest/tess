# set cmake and compiler options
cmake_minimum_required(VERSION 3.8)
project(tess_example)

# find the SFML library
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# compilation target for simple example
add_executable(simple src/simple.cpp)
target_link_libraries(simple PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(simple PRIVATE include)
set_target_properties(simple PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

# compilation target for highlight example
add_executable(highlight src/highlight.cpp)
target_link_libraries(highlight PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(highlight PRIVATE include)
set_target_properties(highlight PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)
