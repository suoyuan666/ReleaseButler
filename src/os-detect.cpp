#include <fstream>
#include <utility>

#include "include/os-detect.h"

namespace os_detect {
auto OsDetect() -> std::optional<enum OS_KIND> {
  std::string os_name;
  {
    std::string buf;
    std::ifstream file{"/etc/os-release"};
    if (!file.is_open()) {
      std::ifstream filet{"/etc/os-release"};
      while(std::getline(filet, buf)){
        if (buf[0] == 'I' && buf[1] == 'D')
        {
          os_name = std::move(buf);
        }
      }
    }else {
      while(std::getline(file, buf)){
        if (buf[0] == 'I' && buf[1] == 'D')
        {
          os_name = std::move(buf);
        }
      }
    }
  }

  if(os_name.empty()){
    return {};
  }

  for(int i = 0; i < OS_KIND; ++i){
    if(os_name == OS_PACKAGE[i].os_kind_){
      switch (i) {
        case 0 : return OS_KIND::debian;
        case 1 : return OS_KIND::ubuntu;
        case 2 : return OS_KIND::fedora;
        case 3 : return OS_KIND::deepin;
        default: return {};
      }
    }
  }

  return {};
}
}  // namespace os_detect