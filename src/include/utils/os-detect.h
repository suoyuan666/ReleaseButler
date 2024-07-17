#include <optional>
#include <string_view>

namespace os_detect {

/**
 * @brief The types of Linux distributions that the current software can detect
 */
constexpr int OS_KIND = 4;

enum OS_KIND {
  debian,
  ubuntu,
  fedora,
  deepin,
};

/**
 * @brief Linux distribution related information.
 */
using packinfo = struct Packinfo {
  std::string_view pack_name_;
  std::string_view pack_install_name_;
  std::string_view os_kind_;
};

/**
 * @brief Information about currently supported Linux distributions
 */
constexpr packinfo OS_PACKAGE[OS_KIND] = {
    {"deb", "dpkg", "debian"},
    {"deb", "dpkg", "ubuntu"},
    {"rpm", "dnf", "fedora"},
    {"deb", "dpkg", "deepin"},
};

/**
 * @brief Used to detect the current Linux distributions.
 *
 * @return A `std::optional<>` class is used to indicate the current system
 * environment.
 */
auto OsDetect(const bool vmode) -> std::optional<enum OS_KIND>;
}  // namespace os_detect