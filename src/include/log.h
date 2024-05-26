#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

[[nodiscard]] auto record2confile(std::string_view url, std::string_view name,
                                std::string_view pack_name,
                                std::string_view version, bool vmode) -> bool;
[[nodiscard]] auto parse_confile_core(std::string_view filename, bool vmode)
    -> bool;
[[nodiscard]] auto parse_confile(std::string_view filename, bool vmode) -> bool;