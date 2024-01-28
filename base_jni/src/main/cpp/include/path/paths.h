
#ifndef PATHS_H_
#define PATHS_H_

#include <string>
#include <utility>

#include "utils/unique_fd.h"

namespace FOREVER {
namespace PATHS {

std::pair<bool, std::string> GetExecutablePath();
std::pair<bool, std::string> GetExecutableDirectoryPath();

// Get the directory to the application's caches directory.
FOREVER::UniqueFD GetCachesDirectory();

std::string JoinPaths(std::initializer_list<std::string> components);

// Returns the absolute path of a possibly relative path.
// It doesn't consult the filesystem or simplify the path.
std::string AbsolutePath(const std::string& path);

// Returns the directory name component of the given path.
std::string GetDirectoryName(const std::string& path);

// Decodes a URI encoded string.
std::string SanitizeURIEscapedCharacters(const std::string& str);

// Converts a file URI to a path.
std::string FromURI(const std::string& uri);

}  // namespace PATHS
}  // namespace FOREVER

#endif  // PATHS_H_
