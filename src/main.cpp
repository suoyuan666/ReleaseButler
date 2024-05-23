#include <sys/types.h>
#include <unistd.h>
// #include <cxxopts.hpp>
#include <boost/program_options.hpp>
#include <boost/process.hpp>
#include <iostream>
// #include <memory>
#include <string>

#include "pack_core.h"

namespace bpo = boost::program_options;
namespace bps = boost::process;

auto main(int argc, char *argv[]) -> int {

  auto env = boost::this_process::environment();
  if(env.count("HOME") == 0){
    std::cerr << "can not read $HOME variable\n";
    return 1;
  }
  auto config_dir = env.at("HOME").to_string();
  config_dir.append("/ReleaseButler/");

  

  return 0;

  bpo::options_description desc("Options");
  desc.add_options()
      ("help,h", "print help message")
      ("install,i", bpo::value<std::string>(), "set package name")
      ("from,f", bpo::value<std::string>(), "set package homepage url")
      ("update,u", "update all package")
      ("verbose,v", "print detailed info")
      ("version", "print version info")
  ;

  bpo::variables_map result;
  bpo::store(bpo::parse_command_line(argc, argv, desc), result);
  bpo::notify(result);

  if (result.count("help") != 0U) {
    std::cout << "ReleaseButler Help: \n\n ReleaseButler --install <package "
                "name> --from <url> [--verbose]\n\n"
              << desc << std::endl;
    return 0;
  }

  if (result.count("version") != 0U) {
    std::cout << "0.1\n";
    return 0;
  }

  std::string install_argu;
  std::string pack_name;
  if (result.count("install") != 0U) {
    install_argu = result["install"].as<std::string>();
  }
  if (result.count("packname") != 0U) {
    pack_name = result["packname"].as<std::string>();
  }

  if(result.count("update") != 0U){
    std::cout << "yeah";
    return 0;
  }

  if(!(install_argu.empty() && pack_name.empty())){
    auto vmode = false;
    if(result.count("verbose") != 0U){
      vmode = true;
    }
    if (!install(install_argu, pack_name, vmode, true)) {
      return 1;
    }
  }

  std::cout << desc << "\n";

  return 0;
}