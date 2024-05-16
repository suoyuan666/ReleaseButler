#include <optional>
#include <string_view>

namespace os_detect {
// constexpr int PACK_NAME_MAX = 12;
// constexpr int OS_NAME_MAX = 12;
constexpr int OS_KIND = 4;

enum OS_KIND{
  debian,
  ubuntu,
  fedora,
  deepin,
};

using packinfo = struct Packinfo {
  std::string_view pack_name_;
  std::string_view pack_install_name_;
  std::string_view os_kind_;
};

constexpr packinfo OS_PACKAGE[OS_KIND] = {
    {"deb", "dpkg", "debian"},
    {"deb", "dpkg", "ubuntu"},
    {"rpm", "dnf", "fedora"},
    {"deb", "dpkg", "deepin"},
};

auto OsDetect() -> std::optional<enum OS_KIND>;
}  // namespace os_detect