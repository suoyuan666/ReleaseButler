#include <sys/types.h>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "pack_core.h"

namespace bpo = boost::program_options;

auto main(int argc, char *argv[]) -> int {
  bpo::options_description desc("Options");
  desc.add_options()
      ("help, h", "print help message")
      ("install, i", bpo::value<std::string>(), "set package name")
      ("from, f", bpo::value<std::string>(), "set package homepage url")
      ("softname,s ", bpo::value<std::string>(), "set release software name")
      ("update, u", "update all package")
      ("parse, p", bpo::value<std::string>(), "set the file name to be parsed")
      ("verbose, v", "print detailed info")
      ("version", "print version info")
  ;

  bpo::variables_map result;
  bpo::store(bpo::parse_command_line(argc, argv, desc), result);
  bpo::notify(result);

  if (result.count("help") != 0U) {
    std::cout << "releasebutler Help: \n\n releasebutler --install <package "
                "name> --from <url> [--verbose]\n\n"
              << desc << std::endl;
    return 0;
  }

  if (result.count("version") != 0U) {
    std::cout << "0.1\n";
    return 0;
  }

  auto vmode{false};
  if(result.count("verbose") != 0U){
    vmode = true;
  }

  if(result.count("parse") != 0U){
    std::string filename = result["parse"].as<std::string>();
    return static_cast<int>(parse_confile(filename, vmode));
  }

  if(result.count("update") != 0U){
    return static_cast<int>(parse_confile("",vmode));
  }

  std::string url;
  std::string pack_name;
  std::string softname;
  if (result.count("from") != 0U) {
    url = result["from"].as<std::string>();
  }
  if (result.count("install") != 0U) {
    pack_name = result["install"].as<std::string>();
  }
  if (result.count("softname") != 0U) {
    softname = result["softname"].as<std::string>();
  }

  if(!(url.empty() && pack_name.empty() && softname.empty())){
    if (!install(url, pack_name, softname, vmode, true)) {
      return 1;
    }
  }

  std::cout << desc << "\n";

  return 0;
}