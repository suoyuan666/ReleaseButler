#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <fstream>
#include <string_view>
#include <boost/process.hpp>

#include "include/pack_core.h"
#include "include/cppcurl.h"
#include "include/os-detect.h"

#include "json.hpp"

namespace fs = std::filesystem;

[[nodiscard]] auto install(std::string url, std::string_view name, std::string_view pack_name, const bool vmode, const bool install) -> bool {
  std::cout << "install it! url: " << url << "\n";
  auto url_token = url.substr(0, 19);
  std::string version;

  // std::string url_bak{url};
  auto url_len = url.length();

  if (url_token == "https://github.com/") {
    if(vmode){
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
    if(vmode){
      std::cout << "location to :" << location << "\n";
    }

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
    if(vmode){
      std::cout << "version: " << version << "\n";
    }

    url.replace(url.rfind("latest"), url.length() + 9, "download/");
    url.append(version);
    url.append("/");
    url.append(pack_name);

    if(vmode){
      std::cout << "download it from the url: " << url << "\n";
    }

    if(!curl.store_ass2file(url, pack_name, vmode)){
      return false;
    }
  }

  if(install && (!install_core(pack_name, vmode))){
      return false;
  }

  if(vmode){
    std::cout << "url_bak_1: " << url.substr(0, url_len) << '\n';
  }

  return record2confile(url.substr(0, url_len), name, pack_name, version, vmode);
}

[[nodiscard]] auto install_core(std::string_view pack_name, const bool vmode) -> bool{
  auto tmp = os_detect::OsDetect();
  if (!tmp.has_value()) {
    std::cerr << "not support your operating system\n";
    return false;
  }

  pid_t pid = fork();
  if(pid == 0){
    std::string path = "/tmp/";
    if (vmode) {
      std::cout << "start install !!!!\n";
    }
    switch (tmp.value()) {
      case os_detect::OS_KIND::debian:
      case os_detect::OS_KIND::ubuntu:
      case os_detect::OS_KIND::deepin:
        path.append(pack_name);
        execl("/usr/bin/sudo", "sudo","dpkg", "-i", path.c_str(), NULL);
        break;
      case os_detect::OS_KIND::fedora:
        path.append(pack_name);
        execl("/usr/bin/sudo", "sudo","dnf", "install", path.c_str(), NULL);
        break;
    }
  }
  wait(nullptr);
  return true;
}

[[nodiscard]] auto record2confile(std::string_view url, std::string_view name,
                                  std::string_view pack_name,
                                  std::string_view version, const bool vmode)
    -> bool {
  if (vmode) {
    std::cout << "url: " << url << "\nname: " << name << "\nversion: " << version << "\n";
  }

  auto env = boost::this_process::environment();
  if(env.count("HOME") == 0){
    std::cerr << "can not read $HOME variable\n";
    return false;
  }
  auto config_file = env.at("HOME").to_string();
  config_file.append("/.config/ReleaseButler/config.json");

  nlohmann::json wdata {
  {
      name , {
        {"name", pack_name},
        {"version", version},
        {"url", url},
        }
    }
  };

  if (vmode) {
    std::cout << "wdata_1:\n" << std::setw(3) << wdata << '\n';
  }

  {
    if (fs::exists(config_file.data()) &&
        (!fs::is_empty(config_file.data()))) {
      nlohmann::json rdata;
      {
        std::ifstream istrm{config_file.data(), std::ios::binary };
        if (istrm.is_open()) {
          rdata = nlohmann::json::parse(istrm);
        } else {
          std::cout << "error";
          return false;
        }
      }
      std::ofstream ostrm{config_file.data(), std::ios::binary};
      if(!rdata.empty()){
        auto [key, val] = wdata.items().begin();
        rdata[key] = val;

        if(vmode){
          std::cout << "wdata_2:\n" <<  std::setw(3) << wdata << '\n';
        }

        ostrm << std::setw(3) << rdata;
      }else{
        ostrm << std::setw(3) << wdata;
      }
    } else {
      std::ofstream ostrm{config_file.data(), std::ios::binary};
      ostrm << std::setw(3) << wdata;
    }
  }

  return true;
}

auto parse_confile(std::string_view filename, const bool vmode) -> bool {
  auto env = boost::this_process::environment();
  if (env.count("HOME") == 0) {
    std::cerr << "can not read $HOME variable\n";
    return false;
  }
  auto config_dir = env.at("HOME").to_string();
  config_dir.append("/.config/ReleaseButler/");

  if (!fs::exists(config_dir)) {
    if (!fs::create_directories(config_dir)) {
      std::cerr << "failed to create directory: " << config_dir << std::endl;
      return false;
    }
  }

  if (!filename.empty()) {
    config_dir.append(filename);
    if(vmode){
      std::cout << "file name to parse: " << filename << '\n';
    }
    return parse_confile_core(config_dir, vmode);
  }

  for (const auto &entry : fs::directory_iterator(config_dir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".json") {
      if (!parse_confile_core(entry.path().string(), vmode)) {
        return false;
      }
    }
  }
  return true;
}

[[nodiscard]] auto parse_confile_core(std::string_view filename ,const bool vmode) -> bool{
  std::ifstream istrm{filename.data(), std::ios::binary};
  nlohmann::json rdata = nlohmann::json::parse(istrm);

  for (const auto& [key, val] : rdata.items()) {
    bool flag_clone{false};
    bool install_flag{true};

    auto ck4flag = [](const nlohmann::json& j, std::string_view val) -> bool{
      bool flag = j.at(val);
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
        std::system(command.c_str());
      } else {
        if(val.count("name") != 0U){
          std::string pack_name = val.at("name");
          if(vmode){
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