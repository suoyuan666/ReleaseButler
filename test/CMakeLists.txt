cmake_minimum_required(VERSION 3.11)

set(TEST_TARGET_NAME main_test)

set(TEST_SOURCE_FILES
    main_test.cpp
)

add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})

set(
    TEST_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
)

include_directories(${TEST_INCLUDE_DIR})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE gtest gtest_main ${ReleaseButler_LIBS})

add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})