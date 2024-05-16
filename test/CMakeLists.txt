cmake_minimum_required(VERSION 3.11)

set(TEST_TARGET_NAME main_test)

set(TEST_SOURCE_FILES
    main_test.cpp
)

# 创建测试目标
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})

set(
    TEST_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
)

# 包含 Google Test 头文件
include_directories(${TEST_INCLUDE_DIR})

# find_package(pack_core REQUIRED)

# 链接 Google Test 库
target_link_libraries(${TEST_TARGET_NAME} PRIVATE gtest gtest_main pack_core)

# 设置测试目标属性
set_target_properties(${TEST_TARGET_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 添加测试
add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})