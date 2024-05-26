set(JSON_BuildTests OFF CACHE INTERNAL "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(googletest)
endif()

add_subdirectory(argparse)