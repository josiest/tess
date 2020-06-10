# adapted from
#   https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-
#   with-sphinx-breathe-doxygen-cmake/

# find sphinx-build and reference it with SPHINX_EXECUTABLE
find_program(SPHINX_EXECUTABLE
    NAMES sphinx-build
    DOC "Path to sphinx-build")

# setup a default error handling routine
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sphinx
    "Failed to find sphinx-build"
    SPHINX_EXECUTABLE)
