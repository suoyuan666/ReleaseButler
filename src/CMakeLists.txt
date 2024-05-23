set(
    SRC_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/third_party/json/single_include/nlohmann
)

set(
    LIB_SOURCE_FILES
    os-detect.cpp
    cppcurl.cpp
    pack_core.cpp
)

include_directories(${SRC_INCLUDE_DIR})

find_package(CURL REQUIRED)

add_library(pack_core OBJECT ${LIB_SOURCE_FILES})

target_link_libraries(pack_core CURL::libcurl)