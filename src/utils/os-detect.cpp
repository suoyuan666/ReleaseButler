#include "utils/os-detect.h"

#include <fstream>
#include <optional>
#include <utility>

#include <tlog.h>

namespace os_detect {
auto OsDetect(const bool vmode) -> std::optional<enum OS_KIND> {
  std::string os_name;
  {
    std::string buf;
    std::ifstream file{"/etc/os-release"};
    if (!file.is_open()) {
      std::ifstream filet{"/etc/os-release"};
      while (std::getline(filet, buf)) {
        if (buf[0] == 'I' && buf[1] == 'D') {
          os_name = std::move(buf);
        }
      }
    } else {
      while (std::getline(file, buf)) {
        if (buf[0] == 'I' && buf[1] == 'D') {
          os_name = buf.substr(3);
        }
      }
    }
  }

  if (os_name.empty()) {
    return {};
  }

  if (vmode) {
    tlog::tprint({"os name: ", os_name}, tlog::tlog_status::DEBUG, tlog::NO_LOG_FILE);
  }

  for (int i = 0; i < OS_KIND; ++i) {
    if (os_name == OS_PACKAGE[i].os_kind_) {
      switch (i) {
        case 0:
          return OS_KIND::debian;
        case 1:
          return OS_KIND::ubuntu;
        case 2:
          return OS_KIND::fedora;
        case 3:
          return OS_KIND::deepin;
        default:
          return std::nullopt;
      }
    }
  }

  return std::nullopt;
}
}  // namespace os_detect