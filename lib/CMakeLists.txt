include_directories(tifini/inc tools/inc extras/inc)

if (HYDRA1COMP)
    include_directories(${HYDRA_INCLUDE_DIR} hydra1comp/inc)
    link_directories(${HYDRA_LIBRARY_DIR})
else()
    include_directories(${HYDRA2_INCLUDE_DIR})
    link_directories(${HYDRA2_LIBRARY_DIR})
endif()

add_subdirectory(tools)
add_subdirectory(extras)
add_subdirectory(tifini)

if (HYDRA1COMP)
    add_subdirectory(hydra1comp)
endif()
