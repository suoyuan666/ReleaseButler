cmake_minimum_required(VERSION 3.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clang-tidy.
set(CMAKE_CXX_STANDARD 20) # Compile as C++20.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++17 support.

project(ReleaseButler
        VERSION 2024.5
        DESCRIPTION "package manager on GitHub"
        LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Setting build type to `Debug` as none was specified.")
        set(CMAKE_BUILD_TYPE "Debug")
endif()

if(EMSCRIPTEN)
        add_compile_options(-fexceptions)
        add_link_options(-fexceptions)
endif()

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT)

if(EXISTS "${PATH_TO_CMAKELISTS_TXT}")
        message(FATAL_ERROR "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake ..\" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif()

# Compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-attributes") # TODO: remove
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=${BUSTUB_SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

enable_testing()

add_executable(ReleaseButler "src/main.cpp")
set(
    RB_SRC_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
)
include_directories(${RB_SRC_INCLUDE_DIR})

add_library(
    cppcurl
    ${PROJECT_SOURCE_DIR}/src/cppcurl.cpp
)

set(ReleaseButler_LIBS
    cppcurl
)

find_package(CURL REQUIRED)
target_link_libraries(ReleaseButler CURL::libcurl ${ReleaseButler_LIBS})

add_subdirectory(third_party)
add_subdirectory(test)