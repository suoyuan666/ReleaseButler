#include <string_view>

/**
 * @brief Simple encapsulation of std::getenv
 *
 * @param name Name of the environment variable
 * @return The value of the environment variable
 */
[[nodiscard]] auto get_env2str(std::string_view name) -> std::string;