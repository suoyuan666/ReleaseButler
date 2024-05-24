cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROGRAM_NAME releasebutler)

project(ReleaseButler
    VERSION 2024.5
    DESCRIPTION "package manager on GitHub"
    LANGUAGES CXX
)

add_subdirectory(src)
add_subdirectory(third_party)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to `Debug` as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fno-omit-frame-pointer -fno-optimize-sibling-calls")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory(test)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build")
    # something come form https://airbus-seclab.github.io/c-compiler-security/clang_compilation.html
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pipe -fPIE -Wall -Wextra -Wpedantic -Werror -Wthread-safety")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fstack-clash-protection -fstack-protector-all -fcf-protection=full")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fvisibility=hidden -fsanitize=cfi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=integer -fsanitize-minimal-runtime -fno-sanitize-recover")
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
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

add_executable(${PROGRAM_NAME} "src/main.cpp")

set(
    RB_SRC_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
)

include_directories(${RB_SRC_INCLUDE_DIR})

set(ReleaseButler_LIBS
    pack_core
)

# find_package(Boost REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
# include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(${PROGRAM_NAME} Boost::program_options ${ReleaseButler_LIBS})
