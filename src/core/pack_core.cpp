#include "core/pack_core.h"

#include <sys/wait.h>
#include <unistd.h>
#include <openssl/sha.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include "tlog.h"
#include "utils/confile.h"
#include "utils/misc.h"

auto install(std::string_view url, std::string_view name,
             std::string_view pack_name, std::optional<std::string_view> sha256_val,
             const bool vmode, const bool install) -> bool {
  tlog::tprint({"install it! url: ", url}, tlog::tlog_status::SUCCESS,
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
    auto rs_version = install_github(url.begin(), name, pack_name, vmode);
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

  if (sha256_val.has_value()) {
    if (sha256_val->size() != SHA256_DIGEST_LENGTH * 2) {
      tlog::tprint({"installl: sha256 value length erro, sha256_val: ",
                    sha256_val.value()},
                   tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
      return false;
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    std::string path {"/tmp/"};
    path.append(pack_name);
    std::ifstream file {path.c_str(), std::ios::binary};
    if (!file.is_open()) {
      tlog::tprint({path, "no suh file"}, tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    }
    std::ostringstream tmp {};
    tmp << file.rdbuf();
    std::string buffer {tmp.str()};

    SHA256_Update(&sha256, buffer.data(), buffer.size());
    std::array<unsigned char, SHA256_DIGEST_LENGTH> hash {};
    SHA256_Final(hash.data(), &sha256);

    std::stringstream sha256_tmp {};
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      sha256_tmp << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash.at(i));
    }

    if (sha256_tmp.str() != sha256_val.value()) {
      tlog::tprint({"installl: sha256 verify failed"}, tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
      return false;
    }
    tlog::tprint({"sha256 verification passed"}, tlog::tlog_status::INFO, tlog::NO_LOG_FILE);
  }

  if (!fetch_enable || (install && (!install_core(pack_name, vmode)))) {
    return false;
  }

  return record2confile(url, name, pack_name, version, vmode);
}

auto install_core(std::string_view pack_name, const bool vmode) -> bool {
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
    path.append(pack_name);
    if (std::filesystem::exists("/usr/bin/sudo")) {
      debug_out4sudo();
      execl("/usr/bin/sudo", PACKAGE_INS_CMD.begin(), path.c_str(), NULL);
    } else if (std::filesystem::exists("/usr/bin/doas")) {
      debug_out4doas();
      execl("/usr/bin/doas", PACKAGE_INS_CMD.begin(), path.c_str(), NULL);
    } else {
      tlog::tprint({"cannot find sudo or doas on /usr/bin"},
                   tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    }
  }
  wait(nullptr);
  return true;
}
