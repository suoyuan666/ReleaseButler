#include "gtest/gtest.h"
#include "pack_core.h"

// 定义一个测试用例
TEST(ExampleTest, Install) {
    // 使用 Google Test 提供的断言来验证结果是否符合预期
    EXPECT_EQ(Install("https://github.com/fastfetch-cli/fastfetch", "fastfetch-linux-amd64.deb"), false); // 期望结果为 3
}

// 可以定义更多的测试用例

// main 函数用于运行所有测试用例
auto main(int argc, char **argv) -> int {
    // 初始化 Google Test 框架
    ::testing::InitGoogleTest(&argc, argv);
    // 运行所有测试用例，并返回测试结果
    return RUN_ALL_TESTS();
}