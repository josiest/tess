# code adapted from
#   https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-
#   with-sphinx-breathe-doxygen-cmake/
# and from
#   https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# define project structure variables
set(HEADER_EXT "hpp")
file(GLOB HAX_HEADERS ${HAX_HEADER_DIR}/*.${HEADER_EXT})

set(DOXYGEN_INPUT_DIR ${HAX_HEADER_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)

# explicitly make the directory so it won't shout warnings at us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)

set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# replace @variables@ in Doxyfile with their respective values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# command for compiling doxygen files
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
    COMMAND Doxygen::doxygen ${DOXYFILE_OUT}        # run doxygen with Doxyfile
    DEPENDS ${HAX_HEADERS}                          # depends on headers
    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}  # depends on Doxyfile.in
    COMMENT "Compiling Doxygen files"
    VERBATIM)

# target for making doxygen files, depends on doxygen xml
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# set up sphinx documentation

# define variables for sphinx project structure
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)
set(EXAMPLE_DIR ${PROJECT_SOURCE_DIR}/examples)
file(GLOB DOC_FILES ${SPHINX_SOURCE}/*.rst)
file(GLOB TEMPLATES ${SPHINX_SOURCE}/_templates/*.html)
file(GLOB EXAMPLE_SOURCES ${EXAMPLE_DIR}/src/*.cpp)
file(GLOB EXAMPLE_IMAGES ${EXAMPLE_DIR}/resources/*.png)
file(MAKE_DIRECTORY ${SPHINX_BUILD})

# define command to compile sphinx docs
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
    COMMAND ${SPHINX_EXECUTABLE} -b html
            -Dbreathe_projects.hax=${DOXYGEN_OUTPUT_DIR}/xml
            ${SPHINX_SOURCE} ${SPHINX_BUILD}

    DEPENDS ${DOC_FILES} ${DOXYGEN_INDEX_FILE}      # depends on .rst, xml,
            ${TEMPLATES} ${EXAMPLE_SOURCES}         #  templates and examples
            ${EXAMPLE_IMAGES}

    MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py        # depends on sphinx config
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}   # compile in build/docs
    COMMENT "Compiling Sphinx files"
    VERBATIM)

add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})
