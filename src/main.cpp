#include <cstddef>
#include <cstdint>
#include <cxxopts.hpp>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "include/cppcurl.h"

[[nodiscard]] auto Install(std::string url) -> bool;

auto main(int argc, char *argv[]) -> int {
  cxxopts::Options options("ReleaseButler", "package manager on GitHub");
  options.add_options()("i,install", "Specify the software to install.",
                        cxxopts::value<std::string>())(
      "u,update", "Try updating all installed software",
      cxxopts::value<bool>()->default_value("false"))(
      "v,version", "Printing software version")("h,help", "Print usage");
  auto result = options.parse(argc, argv);
  if (result.count("help") != 0U) {
    std::cout << options.help() << '\n';
    exit(0);
  }
  std::string bar;
  if (result.count("version") != 0U) {
    std::cout << "0.1\n";
  }
  std::string install_argu;
  if (result.count("install") != 0U) {
    install_argu = result["install"].as<std::string>();
  }
  if (install_argu.empty()) {
    std::cerr << "nooooooooo\n";
    return 1;
  }
  if (Install(install_argu)) {
    return 0;
  }

  return 1;
}

[[nodiscard]] auto Install(std::string url) -> bool {
  std::cout << "install it! url: " << url << "\n";
  auto tmp = url.substr(0, 19);
  if (tmp == "https://github.com/") {
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
    {
      std::shared_ptr<char *> tmp = std::make_shared<char *>();
      curl.getinfo_from_str(CURLINFO_REDIRECT_URL, tmp.get());

      if (!curl.ck4ok()) {
        std::cerr << "connect to link(s) for location failed, error message:"
                  << curl.errorMsg() << std::endl;
        return false;
      }
      location = std::string(*tmp);
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
      for(int i = tmp.length() - 2; i >= 0; --i){
        version += tmp.at(i);
      }
    }
    std::cout << "version: " << version << "\n";

    url.replace(url.rfind("latest"), url.length() + 9, "expanded_assets");
    curl.reset();
    if(curl.empty()){
      std::cerr << "cannot reinit curl" << std::endl;
      return false;
    }

  }
  return true;
}

struct WebSource {
  char *memory;
  size_t size;
};

static auto WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) -> size_t
{
  size_t realsize = size * nmemb;
  auto *mem = static_cast<struct WebSource *>(userp);
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == nullptr) {
    fprintf(stderr, "not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}