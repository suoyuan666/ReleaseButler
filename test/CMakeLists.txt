cmake_minimum_required(VERSION 3.11)

set(TEST_TARGET_NAME main_test)

set(TEST_SOURCE_FILES
    main_test.cpp
)

set(
    TEST_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
)

set (
    TEST_SRC_LIBS
    core
    utils
    curl_cpp
)

set (
    TEST_TP_LIBS
    gtest 
    gtest_main 
    tlog
)

add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})

include_directories(${TEST_INCLUDE_DIR})

target_link_libraries(${TEST_TARGET_NAME} ${TEST_TP_LIBS} ${TEST_SRC_LIBS})

add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})