include_directories(inc)

ROOT_STANDARD_LIBRARY_PACKAGE(TifiniExtras
        DEPENDENCIES ${ROOT_LIBRARIES} Hydra)

# Install the export set for use with the install-tree
install(TARGETS TifiniExtras
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT shlib COMPONENT dev)
