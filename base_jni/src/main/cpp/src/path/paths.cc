
#include "path/paths.h"

#include <sstream>

#include "utils/build_config.h"

namespace FOREVER {
namespace PATHS {

std::string JoinPaths(std::initializer_list<std::string> components) {
  std::stringstream stream;
  size_t i = 0;
  const size_t size = components.size();
  for (const auto& component : components) {
    i++;
    stream << component;
    if (i != size) {
#if FOREVER_OS_WIN
      stream << "\\";
#else   // FOREVER_OS_WIN
      stream << "/";
#endif  // FOREVER_OS_WIN
    }
  }
  return stream.str();
}

std::string SanitizeURIEscapedCharacters(const std::string& str) {
  std::string result;
  result.reserve(str.size());
  for (std::string::size_type i = 0; i < str.size(); ++i) {
    if (str[i] == '%') {
      if (i > str.size() - 3 || !isxdigit(str[i + 1]) ||
          !isxdigit(str[i + 2])) {
        return "";
      }
      const std::string hex = str.substr(i + 1, 2);
      const unsigned char c = strtoul(hex.c_str(), nullptr, 16);
      if (!c) {
        return "";
      }
      result += c;
      i += 2;
    } else {
      result += str[i];
    }
  }
  return result;
}

std::pair<bool, std::string> GetExecutableDirectoryPath() {
  auto path = GetExecutablePath();
  if (!path.first) {
    return {false, ""};
  }
  return {true, FOREVER::PATHS::GetDirectoryName(path.second)};
}

}  // namespace PATHS
}  // namespace FOREVER
