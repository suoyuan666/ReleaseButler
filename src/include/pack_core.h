#include <string>
#include <string_view>

[[nodiscard]] auto install(std::string url, std::string_view name,
                        std::string_view pack_name, bool vmode, bool install)
    -> bool;
[[nodiscard]] auto install_core(std::string_view pack_name, bool vmode)
    -> bool;
[[nodiscard]] auto record2confile(std::string_view url, std::string_view name,
                                std::string_view pack_name,
                                std::string_view version, bool vmode) -> bool;
[[nodiscard]] auto parse_confile_core(std::string_view filename, bool vmode)
    -> bool;
[[nodiscard]] auto parse_confile(std::string_view filename, bool vmode) -> bool;