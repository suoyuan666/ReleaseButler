#include <gtest/gtest.h>
#include "utils/log.h"
#include "core/pack_core.h"

TEST(install_test, install) {
    // EXPECT_EQ(install("https://github.com/fastfetch-cli/fastfetch", "fastfetch", "fastfetch-linux-amd64.deb", true, false), true);
    // EXPECT_EQ(record2confile("https://github.com/fastfetch-cli/fastfetch", "fastfetch-linux-amd64.deb", "v3", true), true);
    EXPECT_EQ(record2confile("https://github.com/fastfetch-cli/fastfetch", "fastfetch", "fastfetch-linux-amd64.deb", "2.13.2", true), true);
    // EXPECT_EQ(parse_confile("example.json", true), true);
}

auto main(int argc, char **argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}