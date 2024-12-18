#include "utils/confile.h"

#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

#include "core/pack_core.h"
#include "json.hpp"
#include "tlog.h"
#include "utils/env.h"

auto conf_modify(nlohmann::json &json, std::string_view filename,
                 const bool vmode) -> bool {
  std::stringstream str;
  str << std::setw(3) << json;
  if (std::filesystem::exists(filename.data()) &&
      (!std::filesystem::is_empty(filename.data()))) {
    nlohmann::json rdata;
    {
      std::ifstream istrm{filename.begin(), std::ios::binary};
      if (istrm.is_open()) {
        rdata = nlohmann::json::parse(istrm);
      } else {
        std::cout << "error";
        return false;
      }
    }
    std::ofstream ostrm_conf{filename.begin(), std::ios::binary};
    if (!rdata.empty()) {
      auto [key, val] = json.items().begin();
      rdata[key] = val;

      if (vmode) {
        tlog::tprint({"json_2: ", str.str()}, tlog::tlog_status::DEBUG,
                     tlog::NO_LOG_FILE);
      }

      ostrm_conf << str.str();
    } else {
      ostrm_conf << str.str();
    }
  } else {
    std::ofstream ostrm{filename.begin(), std::ios::binary};
    ostrm << str.str();
  }
  return true;
}

auto record2confile(std::string_view url, std::string_view name,
                    std::string_view pack_name, std::string_view version,
                    const bool vmode) -> bool {
  if (vmode) {
    tlog::tprint({"url: ", url, "\n name: ", name, "\n version: ", version},
                 tlog::tlog_status::DEBUG, tlog::NO_LOG_FILE);
  }

  auto config_file = get_env2str("HOME");
  if (config_file.empty()) {
    tlog::tprint({"can not read $HOME variable"}, tlog::tlog_status::ERROR,
                 tlog::NO_LOG_FILE);
    return false;
  }
  config_file.append("/.config/ReleaseButler");
  if (!std::filesystem::is_directory(config_file)) {
    std::filesystem::create_directory(config_file);
  }
  config_file.append("/info.json");
  std::cout << "confile: " << config_file << '\n';
  auto pak_fil = config_file.substr(0, config_file.length() - 9);
  pak_fil.append("package.json");

  if (vmode) {
    tlog::tprint({"pak_fil: ", pak_fil}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
  }

  nlohmann::json wdata{{name,
                        {
                            {"pakname", pack_name},
                            {"version", version},
                            {"url", url},
                        }}};

  nlohmann::json pakdata{
      {name, version},
  };

  if (vmode) {
    std::ostringstream str_wdata;
    std::ostringstream str_pakdata;
    str_wdata << std::setw(3) << wdata;
    str_pakdata << std::setw(3) << pakdata;
    tlog::tprint({"wdata_1: ", str_wdata.str()}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
    tlog::tprint({"pakdata_1: ", str_pakdata.str()}, tlog::tlog_status::DEBUG,
                 tlog::NO_LOG_FILE);
  }

  if (!conf_modify(wdata, config_file, vmode)) {
    return false;
  }
  if (!conf_modify(pakdata, pak_fil, vmode)) {
    return false;
  }

  return true;
}

auto parse_confile(std::string_view filename, const bool vmode) -> bool {
  auto config_dir = get_env2str("HOME");
  if (config_dir.empty()) {
    tlog::tprint({"can not read $HOME variable"}, tlog::tlog_status::ERROR,
                 tlog::NO_LOG_FILE);
    return false;
  }
  config_dir.append("/.config/ReleaseButler/");

  if (!std::filesystem::exists(config_dir)) {
    if (!std::filesystem::create_directories(config_dir)) {
      tlog::tprint({"failed to create directory: ", config_dir},
                   tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
      return false;
    }
  }

  if (!filename.empty()) {
    config_dir.append(filename);
    if (vmode) {
      tlog::tprint({"file name to parse: ", filename}, tlog::tlog_status::DEBUG,
                   tlog::NO_LOG_FILE);
    }
    return parse_confile_core(config_dir, vmode);
  }

  for (const auto &entry : std::filesystem::directory_iterator(config_dir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".json") {
      if (entry.path().filename() == "package.json") {
        continue;
      }
      if (!parse_confile_core(entry.path().string(), vmode)) {
        return false;
      }
    }
  }
  return true;
}

auto parse_confile_core(std::string_view filename, const bool vmode) -> bool {
  std::ifstream istrm{filename.begin(), std::ios::binary};
  nlohmann::json rdata = nlohmann::json::parse(istrm);

  for (const auto &[key, val] : rdata.items()) {
    bool flag_clone{false};
    bool install_flag{true};

    auto ck4flag = [](const nlohmann::json &json,
                      std::string_view val) -> bool {
      bool flag = json.at(val);
      return flag;
    };

    if ((val.count("clone") != 0U) && ck4flag(val, "clone")) {
      flag_clone = true;
    }
    if ((val.count("download") != 0U) && ck4flag(val, "download")) {
      install_flag = false;
    }

    if (val.count("build") != 0U) {
      for (const auto &ele : val.at("build")) {
        std::string tmp = ele.dump();
        auto str = tmp.substr(1, tmp.length() - 2);  // remove "
        std::system(str.c_str());
      }
    }

    if (val.count("url") != 0U) {
      std::string url = val.at("url");
      if (flag_clone) {
        std::string command = "git clone ";
        std::string path = " /tmp/";
        path.append(key);
        command.append(url);
        command.append(path);
      } else {
        if (val.count("name") != 0U) {
          std::string pack_name = val.at("name");
          std::optional<std::string_view> sha256 {};
          if (val.count("sha256") != 0U) {
            std::string tmp = val.at("sha256");
            sha256 = tmp;
          }
          if (vmode) {
            tlog::tprint(
                {"url :", url, "\n name: ", key, "\n pack_name: ", pack_name},
                tlog::tlog_status::DEBUG, tlog::NO_LOG_FILE);
            if (sha256.has_value()) {
              tlog::tprint(
                {"sha256 value:", sha256.value()},
                tlog::tlog_status::DEBUG, tlog::NO_LOG_FILE);
            }
          }
          if (!install(url, key, pack_name, sha256, vmode, install_flag)) {
            return false;
          }
        }
      }
    } else {
      tlog::tprint({"need url !!!"}, tlog::tlog_status::ERROR,
                   tlog::NO_LOG_FILE);
      return false;
    }

    if (val.count("install") != 0U) {
      for (const auto &ele : val.at("install")) {
        std::string tmp = ele.dump();
        auto str = tmp.substr(1, tmp.length() - 2);  // remove "
        std::system(str.c_str());
      }
    }
  }
  return true;
}