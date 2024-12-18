set (
    CURL_CPP_SRC_INCLUDE
    ${PROJECT_SOURCE_DIR}/src/include
)

set (
    CURL_CPP_TP_INCLUDE
    ${PROJECT_SOURCE_DIR}/third_party/tlog/include/tlog
)

set (
    CURL_CPP_SRC
    cppcurl.cpp
)

include_directories(${CURL_CPP_SRC_INCLUDE} ${CURL_CPP_TP_INCLUDE})

find_package(CURL REQUIRED)

add_library(
    curl_cpp
    OBJECT
    ${CURL_CPP_SRC}
)

target_link_libraries(curl_cpp PRIVATE CURL::libcurl)
