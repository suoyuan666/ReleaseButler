#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <fstream>
#include <string_view>

#include "include/pack_core.h"
#include "include/cppcurl.h"
#include "include/os-detect.h"

#include "json.hpp"

constexpr std::string_view config_file{"example.json"};

[[nodiscard]] auto install(std::string url, std::string_view pack_name, const bool vmode, const bool install) -> bool {
  std::cout << "install it! url: " << url << "\n";
  auto url_token = url.substr(0, 19);
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

  if(install){
    return install_core(pack_name, vmode);
  }

  return true;
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

[[nodiscard]] auto record2confile(std::string_view url, std::string_view name, std::string_view version, const bool vmode) -> bool{
  if(vmode){
    std::cout << "url: " << url << "\nname: " << name << "\nversion: " << version << "\n";
  }

  nlohmann::json wdata {
  {
      name , {
        {"version", version},
        {"url", url},
        }
    }
  };

  if (vmode) {
    std::cout << "wdata:\n" << std::setw(3) << wdata << '\n';
  }

  {
    if (std::filesystem::exists(config_file.data()) &&
        (!std::filesystem::is_empty(config_file.data()))) {
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
          std::cout << "wdata:\n" <<  std::setw(3) << wdata << '\n';
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

[[nodiscard]] auto parse_confile(const bool vmode) -> bool{
  std::ifstream istrm{config_file.data(), std::ios::binary};
  nlohmann::json rdata = nlohmann::json::parse(istrm);

  for (const auto& [key, val] : rdata.items()) {
    bool flag_clone{false};
    auto ck4clone = [](const nlohmann::json& j) -> bool{
      bool flag = j.at("clone");
      return flag;
    };

    if ((val.count("clone") != 0U) && ck4clone(val)) {
      flag_clone = true;
    }

    if (val.count("url") != 0U) {
      std::string url = val.at("url");
      if (flag_clone) {
        // TODO(debian): git clone it.
        std::string command = "git clone ";
        std::string path = " /tmp/";
        path.append(key);
        command.append(url);
        command.append(path);
        std::system(command.c_str());
      } else {
        if(vmode){
          std::cout << "url: " << url << "\n";
          std::cout << "package name: " << key << "\n";
        }

        if (!install(url, key, vmode, false)) {
          return false;
        }
      }
    } else {
      std::cout << "need url !!!\n";
      return false;
    }
    if (val.count("build") != 0U) {
      for (const auto &ele : val.at("build")) {
        std::string tmp = ele.dump();
        auto str = tmp.substr(1, tmp.length() - 2);  // remove "
        std::system(str.c_str());
      }
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