#include <optional>

namespace os_detect {
constexpr int PACK_NAME_MAX = 12;
constexpr int OS_NAME_MAX = 12;
constexpr int OS_KIND = 3;

using packinfo = struct Packinfo {
  char pack_name_[PACK_NAME_MAX];
  char pack_install_name_[PACK_NAME_MAX];
  char os_kind_[OS_NAME_MAX];
};

constexpr packinfo OS_PACKAGE[OS_KIND] = {
    {"deb", "dpkg", "debian"},
    {"deb", "dpkg", "ubuntu"},
    {"rpm", "dnf", "fedora"},
};

auto OsDetect() -> std::optional<int>;
}  // namespace os_detect