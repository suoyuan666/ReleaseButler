#include <string>

[[nodiscard]] auto Install(std::string url, std::string_view pack_name,  bool vmode) -> bool;
auto record(std::string_view url, std::string_view name, std::string_view version) -> bool;