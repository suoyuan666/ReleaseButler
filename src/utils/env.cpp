#include <cstdlib>

#include "env.h"
#include "log.h"

[[nodiscard]] auto get_env2str(std::string_view name) -> std::string {
  char* tmp = std::getenv(name.data());
  return tmp == nullptr ? std::string("") : std::string(tmp);
}