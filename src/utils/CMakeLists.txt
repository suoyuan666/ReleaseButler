set (
    RB_UTILS_SRC_INCLUDE
    ${PROJECT_SOURCE_DIR}/src/include
)

set (
    RB_UTILS_TP_INCLUDE
    ${PROJECT_SOURCE_DIR}/third_party/json/single_include/nlohmann
    ${PROJECT_SOURCE_DIR}/third_party/tlog/include/tlog
)

include_directories(${RB_UTILS_SRC_INCLUDE} ${RB_UTILS_TP_INCLUDE})

set (
    RB_UTILS_SRC
    env.cpp
    confile.cpp
)

add_library(
    utils
    OBJECT
    ${RB_UTILS_SRC}
)