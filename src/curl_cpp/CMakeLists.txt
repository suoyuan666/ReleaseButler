set (
    CURL_CPP_SRC_INCLUDE
    ${PROJECT_SOURCE_DIR}/src/include
)

set (
    CURL_CPP_SRC
    cppcurl.cpp
)

include_directories(${CURL_CPP_SRC_INCLUDE})

find_package(CURL REQUIRED)

add_library(
    curl_cpp
    OBJECT
    ${CURL_CPP_SRC}
)

target_link_libraries(curl_cpp CURL::libcurl)
