#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "core/pack_core.h"
#include "curl_cpp/cppcurl.h"
#include "json.hpp"
#include "tlog.h"
#include "utils/confile.h"
#include "utils/env.h"
#include "utils/os-detect.h"

auto install_github(std::string url, const std::string_view name,
                    const std::string_view pack_name,
                    const bool vmode) -> std::string {
  std::string version = "";

  cppcurl::CPPCURL curl;
  if (curl.empty()) {
    tlog::tprint({"cannot init curl"}, tlog::tlog_status::ERROR,
                 tlog::NO_LOG_FILE);
    return version;
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
    tlog::tprint({"connect to link(s) for response failed, error message: ",
                  curl.errorMsg()},
                 tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    return version;
  }
  // to get GitHub latest release url, it will 302 redirect to the correct url
  if (response_code != 302) {
    std::ostringstream tmp;
    tmp << "something went wrong with response_code: " << response_code;
    tlog::tprint({tmp.str()}, tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    return version;
  }
  std::string location;
  curl.getinfo_from_str(CURLINFO_REDIRECT_URL, location);
  if (!curl.ck4ok()) {
    tlog::tprint({"connect to link(s) for response failed, error message: ",
                  curl.errorMsg()},
                 tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    return version;
  }

  if (location.empty()) {
    tlog::tprint({"something went wrong with *location"},
                 tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    return version;
  }
  if (vmode) {
    tlog::tprint({"location to: ", location}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
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
    tlog::tprint({"version: ", version}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
  }

  auto config_dir = get_env2str("HOME");
  if (config_dir.empty()) {
    std::cerr << "can not read $HOME variable\n";
    return version;
  }
  config_dir.append("/.config/ReleaseButler/");

  if (!std::filesystem::exists(config_dir)) {
    if (!std::filesystem::create_directories(config_dir)) {
      std::cerr << "failed to create directory: " << config_dir << std::endl;
      return version;
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
        return version;
      }
      if (rdata.count(name) != 0U) {
        if (rdata.count(name) != 0U) {
          std::string curr_version = rdata.at(name);
          if (curr_version >= version) {
            return version;
          }
        } else {
          std::cerr << "missing field\n";
          return version;
        }
      }
    }
  }

  url.replace(url.rfind("latest"), url.length() + 9, "download/");
  url.append(version);
  url.append("/");
  url.append(pack_name);

  if (vmode) {
    tlog::tprint({"download it from the url: ", url}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
  }

  if (!curl.store_ass2file(url, pack_name, vmode)) {
    return version;
  }

  return version;
}