#include "log.h"
#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "env.h"
#include "pack_core.h"
#include "json.hpp"

namespace fs = std::filesystem;

[[nodiscard]] auto conf_modify(nlohmann::json &json, std::string_view filename,
                              const bool vmode) -> bool {
  if (fs::exists(filename.data()) && (!fs::is_empty(filename.data()))) {
    nlohmann::json rdata;
    {
      std::ifstream istrm{filename.data(), std::ios::binary};
      if (istrm.is_open()) {
        rdata = nlohmann::json::parse(istrm);
      } else {
        std::cout << "error";
        return false;
      }
    }
    std::ofstream ostrm_conf{filename.data(), std::ios::binary};
    if (!rdata.empty()) {
      auto [key, val] = json.items().begin();
      rdata[key] = val;

      if (vmode) {
        std::cout << "json_2:\n" << std::setw(3) << json << '\n';
      }

      ostrm_conf << std::setw(3) << rdata;
    } else {
      ostrm_conf << std::setw(3) << json;
    }
  } else {
    std::ofstream ostrm{filename.data(), std::ios::binary};
    ostrm << std::setw(3) << json;
  }
  return true;
}

[[nodiscard]] auto record2confile(std::string_view url, std::string_view name,
                                  std::string_view pack_name,
                                  std::string_view version, const bool vmode)
    -> bool {
  if (vmode) {
    std::cout << "url: " << url << "\nname: " << name
              << "\nversion: " << version << "\n";
  }

  auto config_file = get_env2str("HOME");
  if (config_file.empty()) {
    std::cerr << "can not read $HOME variable\n";
    return false;
  }
  config_file.append("/.config/ReleaseButler/info.json");
  auto pak_fil = config_file.substr(0, config_file.length() - 9);
  pak_fil.append("package.json");

  if (vmode) {
    std::cout << "pak_fil: " << pak_fil << '\n';
  }

  nlohmann::json wdata{
{name, {
    {"name", pack_name},
    {"version", version},
    {"url", url},
      }
    }
  };

  nlohmann::json pakdata{
{name, version},
  };

  if (vmode) {
    std::cout << "wdata_1:\n" << std::setw(3) << wdata << '\n';
    std::cout << "pakdata_1:\n" << std::setw(3) << pakdata << '\n';
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
    std::cerr << "can not read $HOME variable\n";
    return false;
  }
  config_dir.append("/.config/ReleaseButler/");

  if (!fs::exists(config_dir)) {
    if (!fs::create_directories(config_dir)) {
      std::cerr << "failed to create directory: " << config_dir << std::endl;
      return false;
    }
  }

  if (!filename.empty()) {
    config_dir.append(filename);
    if (vmode) {
      std::cout << "file name to parse: " << filename << '\n';
    }
    return parse_confile_core(config_dir, vmode);
  }

  for (const auto &entry : fs::directory_iterator(config_dir)) {
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

[[nodiscard]] auto parse_confile_core(std::string_view filename,
                                      const bool vmode) -> bool {
  std::ifstream istrm{filename.data(), std::ios::binary};
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
          if (vmode) {
            std::cout << "url: " << url << "\n";
            std::cout << "name: " << key << "\n";
            std::cout << "pack_name: " << pack_name << "\n";
          }

          if (!install(url, key, pack_name, vmode, install_flag)) {
            return false;
          }
        }
      }
    } else {
      std::cout << "need url !!!\n";
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