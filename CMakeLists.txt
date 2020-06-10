cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")

# setup the project and include the documentation
project("hax")
add_subdirectory("docs")

# setup the library
FILE(GLOB sources src/*.cpp)
add_library(hax ${sources})
target_include_directories(hax PUBLIC include)
