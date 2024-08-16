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
    ${PROJECT_SOURCE_DIR}/third_party/tlog/include/tlog
)

include_directories(${RB_SRC_INCLUDE_DIR} ${RB_THIRD_PARTY_INCLUDE_DIR})

set(ReleaseButler_LIBS
    core
    utils
    curl_cpp
)

set(
    ReleaseButler_TP_LIBS
    tlog
)

target_link_libraries(${PROGRAM_NAME} ${ReleaseButler_LIBS} ${ReleaseButler_TP_LIBS})