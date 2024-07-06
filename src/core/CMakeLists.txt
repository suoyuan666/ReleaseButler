set (
    RB_CORE_SRC_INCLUDE
    ${PROJECT_SOURCE_DIR}/src/include
)

set (
    RB_CORE_TP_INCLUDE
    ${PROJECT_SOURCE_DIR}/third_party/json/single_include/nlohmann
)

set (
    RB_CORE_SRC
    pack_core.cpp
)

include_directories(${RB_CORE_SRC_INCLUDE} ${RB_CORE_TP_INCLUDE})

add_library(
    core
    OBJECT
    ${RB_CORE_SRC}
)