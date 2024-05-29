#include <sys/types.h>
#include <unistd.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

#include "log.h"
#include "pack_core.h"
#include "misc.h"

auto main(int argc, char *argv[]) -> int {
  argparse::ArgumentParser program(PACK_NAME.data());
  program.add_description(RB_DES.data());
  program.set_usage_max_line_width(80);

  argparse::ArgumentParser install_command("install");
  install_command.add_description("install package form specified url");
  install_command.add_argument("--package").help("set package name, like fastfetch");
  install_command.add_argument("--pakname").help("set package binary name, like fastfetch-linux-amd64.deb");
  install_command.add_argument("-f", "--from").help("set package homepage url");

  argparse::ArgumentParser update_command("update");
  update_command.add_argument("--all").help("update all installed package");
  update_command.add_description("update installed package");

  program.add_argument("--parse").help("set the file name to be parsed");
  program.add_argument("-v", "--verbose")
      .default_value(false)
      .help("set more information output")
      .implicit_value(true)
      .nargs(0);
  program.add_argument("--version").help("print version info").nargs(0);
  program.add_subparser(install_command);
  program.add_subparser(update_command);

  program.parse_args(argc, argv);

  if (program.is_used("--version")) {
    std::cout << RB_MES_PREV << VERSION << std::endl;
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

  if (update_command.is_used("--all")) {
    return static_cast<int>(parse_confile("", vmode));
  }

  std::string url;
  std::string pack_name;
  std::string softname;
  if (install_command.is_used("--from")) {
    url = install_command.get<std::string>("--from");
  }
  if (install_command.is_used("--package")) {
    softname = install_command.get<std::string>("--package");
  }
  if (install_command.is_used("--pakname")) {
    pack_name = install_command.get<std::string>("--pakname");
  }

  if (!(url.empty() && pack_name.empty() && softname.empty())) {
    if (!install(url, pack_name, softname, vmode, true)) {
      return 1;
    }
  }

  std::cout << program << "\n";

  return 0;
}