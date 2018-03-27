include_directories(inc)
link_directories(${PARTICLEEVENT_LIBRARY_DIR})

ROOT_STANDARD_LIBRARY_PACKAGE(Tifini
        DEPENDENCIES ${ROOT_LIBRARIES} ${HYDRA_LIBRARIES} TifiniTools)

# Add all targets to the build-tree export set
export(TARGETS Tifini TifiniTools
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake)

# Install the export set for use with the install-tree
install(TARGETS Tifini
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT shlib COMPONENT dev)
