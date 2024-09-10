#include "core/pack_core.h"

#include <sys/wait.h>
#include <tlog.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <json.hpp>
#include <string>
#include <string_view>

#include "curl_cpp/cppcurl.h"
#include "utils/env.h"
#include "utils/log.h"
#include "utils/os-detect.h"

auto install(std::string_view url, std::string_view name,
             std::string_view pack_name, const bool vmode,
             const bool install) -> bool {
  tlog::tprint({"instalk it! url: ", url}, tlog::tlog_status::SUCCESS,
               tlog::NO_LOG_FILE);
  std::string_view url_token;
  std::string_view version;
  bool fetch_enable = false;

  if (url.length() > 19) {
    url_token = url.substr(0, 19);
  }

  if (url_token == "https://github.com/") {
    fetch_enable = true;
    if (vmode) {
      tlog::tprint({"find it on GitHub"}, tlog::tlog_status::DEBUG,
                   tlog::NO_LOG_FILE);
    }
    auto rs_version = install_github(url.data(), name, pack_name, vmode);
    if (rs_version.empty()) {
      tlog::tprint({"cannot get version"}, tlog::tlog_status::ERROR,
                   tlog::NO_LOG_FILE);
      return false;
    }
    version = rs_version;
    if (vmode) {
      tlog::tprint({"version: ", version}, tlog::tlog_status::DEBUG,
                   tlog::NO_LOG_FILE);
    }
  }

  if (!fetch_enable || (install && (!install_core(pack_name, vmode)))) {
    return false;
  }

  return record2confile(url, name, pack_name, version, vmode);
}

auto install_core(std::string_view pack_name, const bool vmode) -> bool {
  auto tmp = os_detect::OsDetect(vmode);
  if (!tmp.has_value()) {
    tlog::tprint({"not support your operating system"},
                 tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    return false;
  }

  pid_t pid = fork();
  if (pid == 0) {
    auto debug_out4sudo = [&vmode]() {
      if (vmode) {
        tlog::tprint({"it will install by sudo"}, tlog::tlog_status::DEBUG,
                     tlog::NO_LOG_FILE);
      }
    };
    auto debug_out4doas = [&vmode]() {
      if (vmode) {
        tlog::tprint({"it will install by doas"}, tlog::tlog_status::DEBUG,
                     tlog::NO_LOG_FILE);
      }
    };
    std::string path = "/tmp/";
    if (vmode) {
      tlog::tprint({"start install !!!!"}, tlog::tlog_status::DEBUG,
                   tlog::NO_LOG_FILE);
    }
    switch (tmp.value()) {
      case os_detect::OS_KIND::debian:
      case os_detect::OS_KIND::ubuntu:
      case os_detect::OS_KIND::deepin:
        path.append(pack_name);
        if (std::filesystem::exists("/usr/bin/sudo")) {
          debug_out4sudo();
          execl("/usr/bin/sudo", "sudo", "dpkg", "-i", path.c_str(), NULL);
        } else if (std::filesystem::exists("/usr/bin/doas")) {
          debug_out4doas();
          execl("/usr/bin/doas", "doas", "dpkg", "-i", path.c_str(), NULL);
        }
        break;
      case os_detect::OS_KIND::fedora:
        path.append(pack_name);
        if (std::filesystem::exists("/usr/bin/sudo")) {
          debug_out4sudo();
          execl("/usr/bin/sudo", "sudo", "dnf", "install", path.c_str(), NULL);
        } else if (std::filesystem::exists("/usr/bin/doas")) {
          debug_out4doas();
          execl("/usr/bin/doas", "doas", "dnf", "install", path.c_str(), NULL);
        }
        break;
    }
  }
  wait(nullptr);
  return true;
}
