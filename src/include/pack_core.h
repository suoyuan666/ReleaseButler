#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

[[nodiscard]] auto install(std::string url, std::string_view name,
                        std::string_view pack_name, bool vmode, bool install)
    -> bool;
[[nodiscard]] auto install_core(std::string_view pack_name, bool vmode)
    -> bool;