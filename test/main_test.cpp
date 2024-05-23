#include "gtest/gtest.h"
#include "pack_core.h"

TEST(ExampleTest, Install) {
    // EXPECT_EQ(record2confile("https://github.com/fastfetch-cli/fastfetch", "fastfetch-linux-amd64.deb", "v3", true), true);
    // EXPECT_EQ(parse_confile(true), true);
    // EXPECT_EQ(record2confile("https://github.com/fastfetch-cli/fastfetch", "amd64.deb", "v3", true), true);
    EXPECT_EQ(parse_confile(true), true);
}

auto main(int argc, char **argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}