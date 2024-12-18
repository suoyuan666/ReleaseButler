#include <gtest/gtest.h>
#include "utils/confile.h"
#include "core/pack_core.h"

TEST(installtest, install) {
    EXPECT_EQ(install("https://github.com/fastfetch-cli/fastfetch", "fastfetch", "fastfetch-linux-amd64.deb", "5542cec5666645d5562a0c345d75cb6a828b914c5e0f53d8d0deee703667c75f", true, false), true);
    // EXPECT_EQ(install_github("https://github.com/fastfetch-cli/fastfetch", "fastfetch", "fastfetch-linux-amd64.deb", true), "2.32.1");
    // EXPECT_EQ(record2confile("https://github.com/fastfetch-cli/fastfetch", "fastfetch-linux-amd64.deb", "v3", true), true);
    // EXPECT_EQ(record2confile("https://github.com/fastfetch-cli/fastfetch", "fastfetch", "fastfetch-linux-amd64.deb", "2.13.2", true), true);
    EXPECT_EQ(parse_confile("test.json", true), true);
}

auto main(int argc, char **argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}