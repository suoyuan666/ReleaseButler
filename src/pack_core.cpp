#include "include/pack_core.h"
#include <unistd.h>
#include <iostream>
#include <ranges>

#include "include/cppcurl.h"
#include "include/os-detect.h"

[[nodiscard]] auto Install(std::string url, std::string_view pack_name) -> bool {
  std::cout << "install it! url: " << url << "\n";
  auto url_token = url.substr(0, 19);
  if (url_token == "https://github.com/") {
    std::cout << "find it! GitHub\n";

    cppcurl::CPPCURL curl;
    if (curl.empty()) {
      std::cerr << "cannot init curl" << std::endl;
      return false;
    }
    if (*(url.end() - 1) != '/') {
      url += "/releases/latest";
    } else {
      url += "releases/latest";
    }
    curl.setopt(CURLOPT_URL, url);
    curl.perform();

    int64_t response_code;
    curl.getinfo(CURLINFO_RESPONSE_CODE, &response_code);

    if (!curl.ck4ok()) {
      std::cerr << "connect to link(s) for response failed, error message:"
                << curl.errorMsg() << std::endl;
      return false;
    }
    if (response_code != 302) {
      std::cerr << "something went wrong with response_code: " << response_code
                << std::endl;
      return false;
    }
    std::string location;
    curl.getinfo_from_str(CURLINFO_REDIRECT_URL, location);
    if (!curl.ck4ok()) {
      std::cerr << "connect to link(s) for location failed, error message:"
                << curl.errorMsg() << std::endl;
      return false;
    }

    // location = https://github.com/starship/starship/releases/tag/v1.18.2
    if (location.empty()) {
      std::cerr << "something went wrong with *location" << location
                << std::endl;
      return false;
    }
    std::cout << location << "\n";
    std::string version;
    {
      std::string tmp;
      for (auto c : std::ranges::reverse_view{location}) {
        if (c == '/') {
          break;
        }
        tmp += c;
      }
      for (int i = tmp.length() - 1; i >= 0; --i) {
        version += tmp.at(i);
      }
    }
    std::cout << "version: " << version << "\n";

    url.replace(url.rfind("latest"), url.length() + 9, "download/");
    url.append(version);
    url.append("/");
    url.append(pack_name);

    std::cout << "url: " << url << "\n";
    if(!curl.store_ass2file(url, pack_name)){
      return false;
    }

    auto tmp = os_detect::OsDetect();
    if(!tmp.has_value()){
      std::cerr << "not support your operating system\n";
      return false;
    }

    pid_t pid = fork();
    if(pid == 0){
      switch (tmp.value()) {
        case os_detect::OS_KIND::debian:
        case os_detect::OS_KIND::ubuntu:
        case os_detect::OS_KIND::deepin:
          execl("/usr/bin/sudo", "dpkg", "-i", pack_name, NULL);
          break;
        case os_detect::OS_KIND::fedora:
          break;
      }
    }
    
  }
  return true;
}