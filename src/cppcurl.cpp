#include "include/cppcurl.h"
#include <string>
// #include <string_view>
// #include <vector>

namespace cppcurl {

CPPCURL::CPPCURL() { curl_ = curl_easy_init(); }
CPPCURL::CPPCURL(CURL *curl) : curl_(curl) {}
CPPCURL::~CPPCURL() { curl_easy_cleanup(curl_); }

auto CPPCURL::reset() -> void { curl_easy_reset(curl_); }
auto CPPCURL::getinfo(CURLINFO flag, int64_t* val) -> void {
  code_ = curl_easy_getinfo(curl_, flag, val);
}
auto CPPCURL::getinfo_from_str(CURLINFO flag, char** val) -> void{
  code_ = curl_easy_getinfo(curl_, flag, val);
}

auto CPPCURL::setopt(CURLoption option, int64_t val) -> void{
  curl_easy_setopt(curl_, option, val);
}
auto CPPCURL::setopt(CURLoption option, std::string_view val) -> void{
  curl_easy_setopt(curl_, option, val.data());
}

auto CPPCURL::perform() -> void{
  code_ = curl_easy_perform(curl_);
}

auto CPPCURL::ck4ok() -> bool{
  return code_ == CURLE_OK;
}

auto CPPCURL::errorMsg() -> std::string_view{
  return curl_easy_strerror(code_);
}

auto CPPCURL::empty() -> bool{
  return curl_ == nullptr;
}

}  // namespace cppcurl