#pragma once
#include <string_view>


constexpr std::string_view VERSION = "1.0";
constexpr std::string_view PACK_NAME = "releasebutler";
constexpr std::string_view RB_MES_PREV = "releasebutler: ";
constexpr std::string_view RB_DES = "releasebutler: package manager on GitHub.";

#if defined DEBIAN
constexpr std::string_view REMOTE_URL = "https://github.com/suoyuan666/ReleaseButler-pkgs_DEBIAN";
constexpr std::string_view PACKAGE_INS_CMD = "apt install ";
#elif defined OPENSUSE_TUM
constexpr std::string_view REMOTE_URL = "https://github.com/suoyuan666/ReleaseButler-pkgs_OPENSUSE_TUM";
constexpr std::string_view PACKAGE_INS_CMD = "zypper install ";
#endif