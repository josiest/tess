# code adapted from
#   https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-
#   with-sphinx-breathe-doxygen-cmake/
# and from
#   https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
set(SOURCE_EXT "cpp")
set(HEADER_EXT "hpp")

# setup the project and include the documentation
project("hax")

set(HAX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(HAX_PUBLIC_HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB HAX_SOURCES ${HAX_SOURCE_DIR}/*.${SOURCE_EXT})
file(GLOB HAX_PUBLIC_HEADERS ${HAX_PUBLIC_HEADER_DIR}/*.${HEADER_EXT})

# setup the library
add_library(hax ${HAX_SOURCES})
target_include_directories(hax PUBLIC ${HAX_PUBLIC_HEADER_DIR})

# build the documentation
find_package(Doxygen REQUIRED)

# define relevant paths for building documentation
set(DOXYGEN_INPUT_DIR ${HAX_PUBLIC_HEADER_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# replace @variables@ in Doxyfile with their respective values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# explicitly make the directory so it won't shout warnings at us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# command for compiling index.html and doxygen files
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
    DEPENDS ${HAX_PUBLIC_HEADERS}                   # depends on headers
    COMMAND Doxygen::doxygen ${DOXYFILE_OUT}        # run doxygen with Doxyfile
    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}  # depends on Doxyfile.in
    COMMENT "Compiling Doxygen files")

# target for making doxygen files, depends on existence of doxygen index.html
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})
