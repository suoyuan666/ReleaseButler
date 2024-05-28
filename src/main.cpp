#include <sys/types.h>
#include <unistd.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

#include "log.h"
#include "pack_core.h"

auto main(int argc, char *argv[]) -> int {
  argparse::ArgumentParser program("releasebutler");
  program.add_description("releasebutler: package manager on GitHub.");
  program.set_usage_max_line_width(80);

  program.add_argument("-h", "help")
      .default_value(false)
      .help("print help message")
      .implicit_value(true)
      .nargs(0);
  program.add_argument("-i", "--install").help("set package name");
  program.add_argument("--pakname").help("set package name");
  program.add_argument("-f", "--from").help("set package homepage url");
  program.add_argument("--parse").help("set the file name to be parsed");
  program.add_argument("-u", "--update")
      .default_value(false)
      .help("print help message")
      .implicit_value(true)
      .nargs(0);
  program.add_argument("-v", "--verbose")
      .default_value(false)
      .help("print help message")
      .implicit_value(true)
      .nargs(0);
  program.add_argument("--version").help("print version info").nargs(0);

  program.parse_args(argc, argv);

  if (program.is_used("--version")) {
    std::cout << "0.1\n";
    return 0;
  }

  auto vmode{false};
  if (program.is_used("--verbose")) {
    vmode = true;
  }

  if (program.is_used("--parse")) {
    auto filename = program.get<std::string>("parse");
    return static_cast<int>(parse_confile(filename, vmode));
  }

  if (program.is_used("--update")) {
    return static_cast<int>(parse_confile("", vmode));
  }

  std::string url;
  std::string pack_name;
  std::string softname;
  if (program.is_used("--from")) {
    url = program.get<std::string>("--from");
  }
  if (program.is_used("--install")) {
    softname = program.get<std::string>("--install");
  }
  if (program.is_used("--pakname")) {
    pack_name = program.get<std::string>("--pakname");
  }

  if (!(url.empty() && pack_name.empty() && softname.empty())) {
    if (!install(url, pack_name, softname, vmode, true)) {
      return 1;
    }
  }

  std::cout << program << "\n";

  return 0;
}