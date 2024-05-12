#include <cxxopts.hpp>
#include <iostream>

auto main(int argc, char *argv[]) -> int {
  cxxopts::Options options("ReleaseButler", "package manager on GitHub");
  options.add_options()
        ("i,install", "Specify the software to install.", cxxopts::value<std::string>())
        // ("u,update", "Try updating all installed software", cxxopts::value<bool>()->default_value("false"))
        ("u,update", "Try updating all installed software", cxxopts::value<std::string>())
        ("v,version", "Printing software version")
        ("h,help", "Print usage")
    ;
  auto result = options.parse(argc, argv);
  if (result.count("help") != 0U) {
    std::cout << options.help() << '\n';
    exit(0);
  }
  std::string bar;
  if (result.count("version") != 0U) {
    std::cout << "0.1\n";
  }
  // int foo = result["foo"].as<int>();
  return 0;
}