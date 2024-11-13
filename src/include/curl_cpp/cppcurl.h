#pragma once
#include <curl/curl.h>

#include <cstdint>
#include <string>
#include <string_view>

namespace cppcurl {

/**
 * @brief A simple wrapper for libcurl.
 *
 * This class contains only two private members,
 * `CURL *curl_` and `CURLcode code_`, and a simple wrapper for some libcurl
 * functions, which I want to be able to use in other source files in a C++ way
 * for web downloads and such.
 */
class CPPCURL {
 private:
  CURL *curl_;
  CURLcode code_;

 public:
  CPPCURL();
  ~CPPCURL();
  explicit CPPCURL(CURL *curl);
  CPPCURL(const CPPCURL &val);
  CPPCURL(CPPCURL &&val) noexcept;
  auto operator=(const CPPCURL& val) -> CPPCURL&;
  auto operator=(CPPCURL &&val)  noexcept ->CPPCURL&;

  /**
   * @brief Detecting errors in the `code_` field
   */
  [[nodiscard]] auto ck4ok() const -> bool;

  /**
   * @brief Return (`curl_ == nullptr`)
   */
  [[nodiscard]] auto empty() const -> bool;

  /**
   * @brief Simple wrapping of `curl_easy_reset()`
   */
  auto reset() -> void;

  /**
   * @brief Simple encapsulation of `curl_easy_getinfo()`.
   *
   * Call `curl_easy_getinfo()` to get the information and put the return value
   * of `curl_easy_getinfo()` into the code_ member.
   *
   * @param `flag` Setting the type of information to be extracted.
   * @param `val` The int that needs to hold the information
   *
   * @note For more information on `flag`, see
   * https://curl.se/libcurl/c/curl_easy_getinfo.html.
   */
  auto getinfo(CURLINFO flag, int64_t *val) -> void;

  /**
   * @brief Simple encapsulation of `curl_easy_getinfo()`.
   *
   * Call `curl_easy_getinfo()` to get the information and put the return value
   * of `curl_easy_getinfo()` into the code_ member.
   *
   * @param `flag` Setting the type of information to be extracted.
   * @param `val` The std::string that needs to hold the information
   *
   * @note For more information on `flag`, see
   * https://curl.se/libcurl/c/curl_easy_getinfo.html.
   */
  auto getinfo_from_str(CURLINFO flag, std::string &val) -> void;

  /**
   * @brief Simple encapsulation of `curl_easy_setopt()`.
   *
   * Call `curl_easy_setopt()` to set the information.
   *
   * @param `option` What information to set
   * @param `val` std::string holding information
   *
   * @note For more information on `flag`, see
   * https://curl.se/libcurl/c/curl_easy_setopt.html.
   */
  auto setopt(CURLoption option, std::string_view val) -> void;

  /**
   * @brief Access the specified URL and save it to the specified file
   *
   * @param `url` Specified url
   * @param `file_name` Specified file name
   * @param `vmode` Whether to output more information (for debugging purposes)
   * @return Returns a bool indicating whether the function was executed
   * correctly.
   */
  [[nodiscard]] auto store_ass2file(std::string_view url,
                                    std::string_view file_name, bool vmod)
      -> bool;

  /**
   * @brief Simple wrapping of `curl_easy_perform()`
   */
  auto perform() -> void;

  /**
   * @brief Simple wrapping of `curl_easy_strerror()`
   * @return error message
   */
  auto errorMsg() -> std::string_view;
};
}  // namespace cppcurl