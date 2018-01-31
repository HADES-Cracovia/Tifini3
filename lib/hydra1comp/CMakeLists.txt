include_directories(inc)

ROOT_STANDARD_LIBRARY_PACKAGE(TifiniH1Comp
        DEPENDENCIES ${ROOT_LIBRARIES} ${HYDRA_LIBRARIES})

# Add all targets to the build-tree export set
export(TARGETS TifiniH1Comp
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake)

# Install the export set for use with the install-tree
install(TARGETS TifiniH1Comp
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT shlib COMPONENT dev)
