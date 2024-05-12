#include <fstream>
#include <utility>

#include "include/os-detect.h"
#include "include/errmsg.h"

namespace os_detect {
auto OsDetect() -> std::optional<int> {
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
      return i;
    }
  }

  return {};
}
}  // namespace os_detect