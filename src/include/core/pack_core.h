#include <string>
#include <string_view>

/**
 * @brief Programs for download and installation
 *
 * But in fact `install()` does not perform all of the functions
 * involved in installation that are permitted by the software's operation.
 * It's the parsing of the file that implements all of the features allowed for
 * installation.
 *
 * @param `url` The home page of the website where the program is hosted
 * @param `name` Name of the software
 * @param `pack_name` Name of the package to download, e.g.
 * `fastfetch-linux-amd64.deb`
 * @param `vmode` Whether to output more information (for debugging purposes)
 * @param `install` Whether to call the corresponding package manager to install
 * after download
 * @return  Returns a bool indicating whether the function was executed
 * correctly.
 */
[[nodiscard]] auto install(std::string url, std::string_view name,
                            std::string_view pack_name, bool vmode, bool install)
    -> bool;
/**
 * @brief Core functions that implement the installation functionality
 *
 * @param `pack_name` Name of the package to download, e.g.
 * `fastfetch-linux-amd64.deb`
 * @param `vmode` Whether to output more information (for debugging purposes)
 * @return  Returns a bool indicating whether the function was executed
 * correctly.
 */
[[nodiscard]] auto install_core(std::string_view pack_name, bool vmode) -> bool;