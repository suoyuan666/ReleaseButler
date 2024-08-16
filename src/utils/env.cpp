#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "utils/env.h"

[[nodiscard]] auto get_env2str(std::string_view name) -> std::string {
  char* tmp = std::getenv(name.data());
  return tmp == nullptr ? std::string("") : std::string(tmp);
}

[[nodiscard]] auto get_val_from_std() -> std::string{
  std::string rs;
  std::string key;

  std::getline(std::cin, key);
  std::istringstream str{key};
  if(!(str >> rs)) {
    return "";
  }
  return rs;
}