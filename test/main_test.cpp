#include "gtest/gtest.h"

// 定义一个测试用例
TEST(ExampleTest, Addition) {
    // 在这里编写测试逻辑
    int result = 1 + 2;
    // 使用 Google Test 提供的断言来验证结果是否符合预期
    EXPECT_EQ(result, 3); // 期望结果为 3
}

// 可以定义更多的测试用例

// main 函数用于运行所有测试用例
auto main(int argc, char **argv) -> int {
    // 初始化 Google Test 框架
    ::testing::InitGoogleTest(&argc, argv);
    // 运行所有测试用例，并返回测试结果
    return RUN_ALL_TESTS();
}