# code adapted from
#   https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-
#   with-sphinx-breathe-doxygen-cmake/
# and from
#   https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

find_package(Doxygen REQUIRED)

message("docs binary dir: ${CMAKE_CURRENT_BINARY_DIR}")

# define relevant paths for building documentation
set(DOXYGEN_INPUT_DIR ${HAX_PUBLIC_HEADER_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
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

# set up sphinx
find_package(Sphinx REQUIRED)
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
file(MAKE_DIRECTORY ${SPHINX_BUILD})

add_custom_target(Sphinx ALL
    COMMAND ${SPHINX_EXECUTABLE} -b html ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Compiling Sphinx files")
