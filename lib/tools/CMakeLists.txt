include_directories(inc)

ROOT_STANDARD_LIBRARY_PACKAGE(TifiniTools
        DEPENDENCIES ${ROOT_LIBRARIES} ${HYDRA_LIBRARIES} ${HYDRA1COMP_LIB})

# Add all targets to the build-tree export set
export(TARGETS TifiniTools
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake)

# Install the export set for use with the install-tree
install(TARGETS TifiniTools
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT shlib COMPONENT dev)

find_package(cppunit)

if (cppunit_FOUND)
    subdirs(tests)
endif()
