#include "pack_core.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include "cppcurl.h"
#include "env.h"
#include "json.hpp"
#include "log.h"
#include "os-detect.h"

[[nodiscard]] auto install(std::string url, std::string_view name,
                           std::string_view pack_name, const bool vmode,
                           const bool install) -> bool {
  std::cout << "install it! url: " << url << "\n";
  auto url_token = url.substr(0, 19);
  std::string version;

  auto url_len = url.length();

  if (url_token == "https://github.com/") {
    if (vmode) {
      std::cout << "find it! GitHub\n";
    }

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
    // to get GitHub latest release url, it will 302 redirect to the correct url
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

    if (location.empty()) {
      std::cerr << "something went wrong with *location" << location
                << std::endl;
      return false;
    }
    if (vmode) {
      std::cout << "location to :" << location << "\n";
    }

    {
      std::string tmp;
      // eg: https://github.com/fastfetch-cli/fastfetch/releases/tag/2.17.2
      // this is to get the version at the end of the url, in this case 2.17.2
      //
      // C++ 20 provides `std::ranges::reverse_view` to simplify its writing,
      // but since I don’t know much about other new features of C++ 20, I will
      // give up using ranges for now.
      for (auto c = location.rbegin(); c != location.rend(); ++c) {
        if (*c == '/') {
          break;
        }
        tmp += *c;
      }
      for (int i = tmp.length() - 1; i >= 0; --i) {
        version += tmp.at(i);
      }
    }
    if (vmode) {
      std::cout << "version: " << version << "\n";
    }

    auto config_dir = get_env2str("HOME");
    if (config_dir.empty()) {
      std::cerr << "can not read $HOME variable\n";
      return false;
    }
    config_dir.append("/.config/ReleaseButler/");

    if (!std::filesystem::exists(config_dir)) {
      if (!std::filesystem::create_directories(config_dir)) {
        std::cerr << "failed to create directory: " << config_dir << std::endl;
        return false;
      }
    }

    config_dir.append("package.json");
    if (std::filesystem::exists(config_dir.data()) &&
        (!std::filesystem::is_empty(config_dir.data()))) {
      nlohmann::json rdata;
      {
        std::ifstream istrm{config_dir.data(), std::ios::binary};
        if (istrm.is_open()) {
          rdata = nlohmann::json::parse(istrm);
        } else {
          std::cout << "error";
          return false;
        }
        if (rdata.count(name) != 0U) {
          if (rdata.count(name) != 0U) {
            std::string curr_version = rdata.at(name);
            if (curr_version >= version) {
              return true;
            }
          } else {
            std::cerr << "missing field\n";
            return false;
          }
        }
      }
    }

    url.replace(url.rfind("latest"), url.length() + 9, "download/");
    url.append(version);
    url.append("/");
    url.append(pack_name);

    if (vmode) {
      std::cout << "download it from the url: " << url << "\n";
    }

    if (!curl.store_ass2file(url, pack_name, vmode)) {
      return false;
    }
  }

  if (install && (!install_core(pack_name, vmode))) {
    return false;
  }

  if (vmode) {
    std::cout << "url_bak_1: " << url.substr(0, url_len) << '\n';
  }

  return record2confile(url.substr(0, url_len), name, pack_name, version,
                        vmode);
}

[[nodiscard]] auto install_core(std::string_view pack_name,
                                const bool vmode) -> bool {
  auto tmp = os_detect::OsDetect();
  if (!tmp.has_value()) {
    std::cerr << "not support your operating system\n";
    return false;
  }

  pid_t pid = fork();
  if (pid == 0) {
    std::string path = "/tmp/";
    if (vmode) {
      std::cout << "start install !!!!\n";
    }
    switch (tmp.value()) {
      case os_detect::OS_KIND::debian:
      case os_detect::OS_KIND::ubuntu:
      case os_detect::OS_KIND::deepin:
        path.append(pack_name);
        if (std::filesystem::exists("/usr/bin/sudo")) {
          execl("/usr/bin/sudo", "sudo", "dpkg", "-i", path.c_str(), NULL);
        } else if (std::filesystem::exists("/usr/bin/doas")) {
          execl("/usr/bin/doas", "doas", "dpkg", "-i", path.c_str(), NULL);
        }
        break;
      case os_detect::OS_KIND::fedora:
        path.append(pack_name);
        if (std::filesystem::exists("/usr/bin/sudo")) {
          execl("/usr/bin/sudo", "sudo", "dnf", "install", path.c_str(), NULL);
        } else if (std::filesystem::exists("/usr/bin/doas")) {
          execl("/usr/bin/doas", "doas", "dnf", "install", path.c_str(), NULL);
        }
        break;
    }
  }
  wait(nullptr);
  return true;
}
