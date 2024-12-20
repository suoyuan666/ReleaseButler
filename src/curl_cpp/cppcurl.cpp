#include "curl_cpp/cppcurl.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include "tlog.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

namespace cppcurl {

auto WriteCallback(void *contents, size_t size, size_t nmemb,
                   void *userp) -> size_t {
  auto *ofs = static_cast<std::ofstream *>(userp);
  ofs->write(static_cast<char *>(contents), size * nmemb);
  return size * nmemb;
}

CPPCURL::CPPCURL() : curl_(curl_easy_init()), code_() {}
CPPCURL::CPPCURL(CURL *curl) : curl_(curl), code_() {}
CPPCURL::~CPPCURL() { curl_easy_cleanup(curl_); }

CPPCURL::CPPCURL(const CPPCURL &val) {
  if (val.ck4ok()) {
    this->curl_ = val.curl_;
  }
  if (val.empty()) {
    this->code_ = val.code_;
  }
}

CPPCURL::CPPCURL(CPPCURL &&val) noexcept {
  if (val.ck4ok()) {
    this->curl_ = val.curl_;
  }
  if (val.empty()) {
    this->code_ = val.code_;
  }
}

auto CPPCURL::operator=(const CPPCURL &val) -> CPPCURL & {
  if (val.ck4ok()) {
    this->curl_ = val.curl_;
  }
  if (val.empty()) {
    this->code_ = val.code_;
  }
  return *this;
}

auto CPPCURL::operator=(CPPCURL &&val) noexcept -> CPPCURL & {
  if (val.ck4ok()) {
    this->curl_ = val.curl_;
  }
  if (val.empty()) {
    this->code_ = val.code_;
  }
  return *this;
}

auto CPPCURL::reset() -> void { curl_easy_reset(curl_); }

auto CPPCURL::getinfo(CURLINFO flag, int64_t *val) -> void {
  code_ = curl_easy_getinfo(curl_, flag, val);
}

auto CPPCURL::getinfo_from_str(CURLINFO flag, std::string &val) -> void {
  std::shared_ptr<char *> tmp = std::make_shared<char *>();
  code_ = curl_easy_getinfo(curl_, flag, tmp.get());
  val = *tmp;
}

auto CPPCURL::setopt(CURLoption option, std::string_view val) -> void {
  curl_easy_setopt(curl_, option, val.begin());
}

auto CPPCURL::perform() -> void { code_ = curl_easy_perform(curl_); }

auto CPPCURL::ck4ok() const -> bool { return code_ == CURLE_OK; }

auto CPPCURL::errorMsg() -> std::string_view {
  return curl_easy_strerror(code_);
}

auto CPPCURL::empty() const -> bool { return curl_ == nullptr; }

[[nodiscard]] auto CPPCURL::store_ass2file(std::string_view url,
                                           std::string_view file_name,
                                           const bool vmode) -> bool {
  std::string path = "/tmp/";
  path.append(file_name);

  if (vmode) {
    tlog::tprint({"store it to: ", path}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
  }

  std::ofstream ofs{path.data(), std::ios::binary};
  if (curl_ != nullptr) {
    curl_easy_reset(curl_);
  }
  if (curl_ == nullptr) {
    tlog::tprint({"cannot reinit curl"}, tlog::tlog_status::ERROR,
                 tlog::NO_LOG_FILE);
    return false;
  }

  // set url and view its url after 302 redirection.
  curl_easy_setopt(curl_, CURLOPT_URL, url.begin());
  code_ = curl_easy_perform(curl_);
  int64_t response_code = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
  if (vmode) {
    std::ostringstream str;
    str << "response_code: " << response_code;
    tlog::tprint({str.str()}, tlog::tlog_status::DEBUG, tlog::NO_LOG_FILE);
  }

  // since i prefer not to use `char*`, I do it.
  std::string location;
  {
    std::shared_ptr<char *> redirect = std::make_shared<char *>();
    curl_easy_getinfo(curl_, CURLINFO_REDIRECT_URL, redirect.get());
    if (vmode) {
      tlog::tprint({"redirect url: ", *redirect}, tlog::tlog_status::DEBUG,
                   tlog::NO_LOG_FILE);
    }
    location = *redirect;
  }
  curl_easy_reset(curl_);

  // start saving
  curl_easy_setopt(curl_, CURLOPT_URL, location.c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &ofs);
  code_ = curl_easy_perform(curl_);

  if (code_ != CURLE_OK) {
    tlog::tprint({"store it to: ", path}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
    std::cerr << "connect to link(s) for get information failed, error message:"
              << curl_easy_strerror(code_) << '\n';
    return false;
  }
  return true;
}

}  // namespace cppcurl