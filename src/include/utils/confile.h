#include <string_view>

/**
 * @brief Save the passed parameters to the specified file for storage
 *
 * @note File name cannot be specified
 *
 * @param `url` URL to store
 * @param `name` Name to store
 * @param `pack_name` Package name to store
 * @param `version` Version to store
 * @param `vmode` Whether to output more information (for debugging purposes)
 * @return Returns a bool indicating whether the function was executed correctly.
 */
[[nodiscard]] auto record2confile(std::string_view url, std::string_view name,
                                std::string_view pack_name,
                                std::string_view version, bool vmode) -> bool;

/**
 * @brief Parse the passed file name and start downloading and installing, etc.
 *
 * Because it is necessary to realize the function of traversing all files in the folder, 
 * the specific download and installation functions are implemented in `parse_confile_core()`
 *
 * @param `filename` File name to parse. If the file name is empty, iterate through all json files in the `~/.config/ReleaseButler/` folder
 * @param `vmode` Whether to output more information (for debugging purposes)
 * @return Returns a bool indicating whether the function was executed correctly.
 */
[[nodiscard]] auto parse_confile(std::string_view filename, bool vmode) -> bool;

/**
 * @brief The core part of `parse_confile()`
 *
 * @param `filename` File name to parse
 * @param `vmode` Whether to output more information (for debugging purposes)
 * @return Returns a bool indicating whether the function was executed correctly.
 */
[[nodiscard]] auto parse_confile_core(std::string_view filename, bool vmode)
    -> bool;