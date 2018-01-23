include_directories(tifini/inc tools/inc extras/inc ${PARTICLEEVENT_INCLUDE_DIR} ${HYDRA_INCLUDE_DIR})
link_directories(${HYDRA_LIBRARY_DIR})

add_subdirectory(tools)
add_subdirectory(extras)
add_subdirectory(tifini)

