# set cmake and compiler options
cmake_minimum_required(VERSION 3.10)
project(tess VERSION 0.2.0)

# variables for install destinations
include(GNUInstallDirs)

#
# Define the library
#

# add the interface library target
add_library(tess INTERFACE)
add_library(tess::tess ALIAS tess)
target_compile_features(tess INTERFACE cxx_std_23)

target_include_directories(
    # generate INTERFACE_INCLUDE_DIRECTORIES for the tess target
    tess INTERFACE
    # include the source header files when building
    "$<BUILD_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
    # include the gnu c header files when installing
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_sources(tess INTERFACE
    $<BUILD_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/include/tess/basis.hpp>
    $<BUILD_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/include/tess/hex.hpp>
    $<BUILD_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/include/tess/math.hpp>
    $<BUILD_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/include/tess/point.hpp>
    $<BUILD_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/include/tess/tess.hpp>)

#
# Export and install library targets
#

install(TARGETS tess
        EXPORT tess-targets
        # install target headers into gnu include dir
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

export(EXPORT tess-targets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/tess-targets.cmake
       NAMESPACE tess::)

install(EXPORT tess-targets
        FILE tess-targets.cmake
        NAMESPACE tess::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tess)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#
# Create and install package
#

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/tess-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/tess-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/tess-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBIDIR}/cmake/tess)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/tess-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/tess-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tess)

export(PACKAGE tess)

# Add an option to compile the documentation
option(DOCS "Compile documentation" OFF)
if (DOCS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/docs)
endif()

# Add an option to compile tests
option(TESTING "Compile tests" OFF)
if (TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()
