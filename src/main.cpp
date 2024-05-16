#include <sys/types.h>
#include <unistd.h>
#include <cxxopts.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "pack_core.h"

auto main(int argc, char *argv[]) -> int {
  cxxopts::Options options("ReleaseButler", "package manager on GitHub");
  options.add_options()
    ("i,install", "Specify the software to install.",  cxxopts::value<std::string>())
    ("u,update", "Try updating all installed software",  cxxopts::value<bool>()->default_value("false"))
    ("p, packname", "Specify the name of the package to download", cxxopts::value<std::string>())
    ("v,version", "Printing software version")("h,help", "Print usage");
  auto result = options.parse(argc, argv);

  if (result.count("help") != 0U) {
    std::cout << options.help() << '\n';
    exit(0);
  }
  if (result.count("version") != 0U) {
    std::cout << "0.1\n";
  }

  std::string install_argu;
  if (result.count("install") != 0U) {
    install_argu = result["install"].as<std::string>();
  }
  if (install_argu.empty()) {
    std::cerr << "install!!!!!\n";
    return 1;
  }

  std::string pack_name;
  if (result.count("packname") != 0U) {
    pack_name = result["packname"].as<std::string>();
  }
  if (pack_name.empty()) {
    std::cerr << "packname!!!!!\n";
    return 1;
  }
  if (!Install(install_argu, pack_name)) {
    return 1;
  }

  return 0;
}