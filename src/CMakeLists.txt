# set(
#     SRC_INCLUDE_DIR
#     ${PROJECT_SOURCE_DIR}/src/include
#     ${PROJECT_SOURCE_DIR}/third_party/json/single_include/nlohmann
# )

# set(
#     LIB_SOURCE_FILES
#     os-detect.cpp
#     cppcurl.cpp
#     pack_core.cpp
#     log.cpp
#     env.cpp
# )

# include_directories(${SRC_INCLUDE_DIR})

# find_package(CURL REQUIRED)

# add_library(pack_core OBJECT ${LIB_SOURCE_FILES})

# target_link_libraries(pack_core CURL::libcurl)

add_subdirectory(core)
add_subdirectory(curl_cpp)
add_subdirectory(utils)

set(PROGRAM_NAME releasebutler)

add_executable(${PROGRAM_NAME} "main.cpp")

set(
    RB_SRC_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
)

set(
    RB_THIRD_PARTY_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/third_party/argparse/include
)

include_directories(${RB_SRC_INCLUDE_DIR} ${RB_THIRD_PARTY_INCLUDE_DIR})

set(ReleaseButler_LIBS
    core
    utils
    curl_cpp
)

target_link_libraries(${PROGRAM_NAME} ${ReleaseButler_LIBS})