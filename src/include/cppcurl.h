#include <curl/curl.h>

#include <cstdint>
#include <string>
#include <string_view>

namespace cppcurl {

class CPPCURL {
 private:
  CURL *curl_;
  CURLcode code_;

 public:
  CPPCURL();
  ~CPPCURL();
  explicit CPPCURL(CURL *curl);
  // check code_ field
  auto ck4ok() -> bool;
  // return (curl_ == nullptr)
  auto empty() -> bool;
  // reset curl_ field
  auto reset() -> void;
  auto getinfo(CURLINFO flag, int64_t *val) -> void;
  auto getinfo_from_str(CURLINFO flag, std::string &val) -> void;
  auto setopt(CURLoption option, int64_t val) -> void;
  auto setopt(CURLoption option, std::string_view val) -> void;
  [[nodiscard]] auto store_ass2file(std::string_view url,
                                    std::string_view file_name, bool vmod)
      -> bool;
  auto perform() -> void;
  auto errorMsg() -> std::string_view;
};
}  // namespace cppcurl