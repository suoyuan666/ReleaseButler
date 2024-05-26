#include "include/cppcurl.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

namespace cppcurl {

auto WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    -> size_t {
  auto ofs = static_cast<std::ofstream *>(userp);
  ofs->write(static_cast<char *>(contents), size * nmemb);
  return size * nmemb;
}

CPPCURL::CPPCURL() { curl_ = curl_easy_init(); }
CPPCURL::CPPCURL(CURL *curl) : curl_(curl) {}
CPPCURL::~CPPCURL() { curl_easy_cleanup(curl_); }

auto CPPCURL::reset() -> void { curl_easy_reset(curl_); }
auto CPPCURL::getinfo(CURLINFO flag, int64_t *val) -> void {
  code_ = curl_easy_getinfo(curl_, flag, val);
}
auto CPPCURL::getinfo_from_str(CURLINFO flag, std::string &val) -> void {
  std::shared_ptr<char *> tmp = std::make_shared<char *>();
  code_ = curl_easy_getinfo(curl_, flag, tmp.get());
  val = *tmp;
}

auto CPPCURL::setopt(CURLoption option, int64_t val) -> void {
  curl_easy_setopt(curl_, option, val);
}
auto CPPCURL::setopt(CURLoption option, std::string_view val) -> void {
  curl_easy_setopt(curl_, option, val.data());
}

auto CPPCURL::perform() -> void { code_ = curl_easy_perform(curl_); }

auto CPPCURL::ck4ok() -> bool { return code_ == CURLE_OK; }

auto CPPCURL::errorMsg() -> std::string_view {
  return curl_easy_strerror(code_);
}

auto CPPCURL::empty() -> bool { return curl_ == nullptr; }

[[nodiscard]] auto CPPCURL::store_ass2file(std::string_view url,
                                           std::string_view file_name,
                                           const bool vmode) -> bool {
  std::string path = "/tmp/";
  path.append(file_name);

  if (vmode) {
    std::cout << "store it to " << path << std::endl;
  }

  std::ofstream ofs{path.data(), std::ios::binary};
  if (curl_ != nullptr) {
    curl_easy_reset(curl_);
  }
  if (curl_ == nullptr) {
    std::cerr << "cannot reinit curl" << std::endl;
    return false;
  }

  curl_easy_setopt(curl_, CURLOPT_URL, url.data());
  code_ = curl_easy_perform(curl_);
  int64_t response_code;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
  if (vmode) {
    std::cout << "response_code: " << response_code << "\n";
  }

  std::string location;
  {
    std::shared_ptr<char *> tmp = std::make_shared<char *>();
    curl_easy_getinfo(curl_, CURLINFO_REDIRECT_URL, tmp.get());
    if (vmode) {
      std::cout << *tmp << "\n";
    }
    location = *tmp;
  }
  curl_easy_reset(curl_);

  curl_easy_setopt(curl_, CURLOPT_URL, location.c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &ofs);
  code_ = curl_easy_perform(curl_);

  if (code_ != CURLE_OK) {
    std::cerr << "connect to link(s) for get information failed, error message:"
              << curl_easy_strerror(code_) << std::endl;
    return false;
  }
  return true;
}

}  // namespace cppcurl