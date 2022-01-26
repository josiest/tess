# --- compile the library ---

# add the interface library target
add_library(tess INTERFACE)
add_library(tess::tess ALIAS tess)

# --- install and export the library ---

target_include_directories(
    # generate INTERFACE_INCLUDE_DIRECTORIES for the tess target
    tess INTERFACE
    # include the source header files when building
    "$<BUILD_INTERFACE:${tess_INCLUDE_DIR}>"
    # include the gnu c header files when installing
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(
    TARGETS tess EXPORT tessTargets
    # install target headers into gnu include dir
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the header source files into gnu include dir
install(DIRECTORY ${tess_INCLUDE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tess
        FILES_MATCHING PATTERN *.hpp)

# install the target's export details to make it easier for other cmake
# programs to work with it
install(EXPORT tessTargets
        FILE tessTargets.cmake
        NAMESPACE tess::
        # install into the gnu lib dir in the cmake subfolder
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tess)
