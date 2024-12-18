#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <optional>
#include <string>

#include "argparse/argparse.hpp"
#include "core/pack_core.h"
#include "tlog.h"
#include "utils/confile.h"
#include "utils/misc.h"

auto main(int argc, char *argv[]) -> int {
  argparse::ArgumentParser program("releasebutler");
  program.add_description("package manager on GitHub.");
  program.set_usage_max_line_width(80);

  argparse::ArgumentParser install_command("install", VERSION.begin(),
                                           argparse::default_arguments::help);
  install_command.add_description("install package form specified url");
  install_command.add_argument("--package")
      .help("set package name, like `fastfetch`")
      .metavar("package");
  install_command.add_argument("--packname")
      .help("set package binary name, like `fastfetch-linux-amd64.deb`")
      .metavar("package name");
  install_command.add_argument("-f", "--from")
      .help("set package homepage url")
      .metavar("url");

  argparse::ArgumentParser update_command("update", VERSION.begin(),
                                          argparse::default_arguments::help);
  update_command.add_argument("--all").help("update all installed package");
  update_command.add_argument("--package")
      .help("set package name, like `fastfetch`")
      .metavar("package");
  update_command.add_description("update installed package");

  argparse::ArgumentParser uninstall_command("uninstall", VERSION.begin(),
                                             argparse::default_arguments::help);
  uninstall_command.add_description("uninstall package");
  uninstall_command.add_argument("--package")
      .help("set package name, like `fastfetch`")
      .metavar("package");

  program.add_argument("--version")
      .help("shows version info")
      .nargs(0)
      .action([](const std::string &) {
        std::cout << RB_MES_PREV << VERSION << '\n';
      });
  program.add_argument("--parse")
      .help("set the file name to be parsed")
      .metavar("file name");
  program.add_argument("-v", "--verbose")
      .default_value(false)
      .help("set more information output")
      .implicit_value(true)
      .nargs(0);
  program.add_subparser(install_command);
  program.add_subparser(uninstall_command);
  program.add_subparser(update_command);

  program.parse_args(argc, argv);

  auto vmode{false};
  if (program.is_used("--verbose")) {
    vmode = true;
    tlog::tprint({"set verbose to on"}, tlog::tlog_status::INFO,
                 tlog::NO_LOG_FILE);
  }

  if (program.is_used("--parse")) {
    auto filename = program.get<std::string>("parse");
    tlog::tprint(
        {"start to parse config file in $HOME/.config/ReleaseButler/*.json"},
        tlog::tlog_status::INFO, tlog::NO_LOG_FILE);
    return static_cast<int>(parse_confile(filename, vmode));
  }

  if (update_command.is_used("--all")) {
    tlog::tprint({"start to update all package"}, tlog::tlog_status::INFO,
                 tlog::NO_LOG_FILE);
    return static_cast<int>(parse_confile("", vmode));
  }

  std::string url;
  std::string pack_name;
  std::string package;
  bool install_enable{false};
  if (install_command.is_used("--from")) {
    install_enable = true;
    url = install_command.get<std::string>("--from");
  }
  if (install_command.is_used("--package")) {
    install_enable = true;
    package = install_command.get<std::string>("--package");
  }
  if (install_command.is_used("--packname")) {
    install_enable = true;
    pack_name = install_command.get<std::string>("--packname");
  }

  if (install_enable) {
    if (!(url.empty() || pack_name.empty() || package.empty())) {
      if (!install(url, package, pack_name, std::nullopt, vmode, true)) {
        return 1;
      }
    } else {
      tlog::tprint(
          {"`--from`, `--package` and `--packname` must appear together"},
          tlog::tlog_status::ERROR, tlog::NO_LOG_FILE);
    }
  }

  return 0;
}
