
#ifndef UNIQUE_FD_H_
#define UNIQUE_FD_H_

#include "build_config.h"
#include "unique_object.h"

#if FOREVER_OS_WIN
#include <windows.h>
#include <map>
#include <mutex>
#include <optional>
#else  // FOREVER_OS_WIN
#include <dirent.h>
#include <unistd.h>
#endif  // FOREVER_OS_WIN

namespace FOREVER {
namespace INTERNAL {

#if FOREVER_OS_WIN

namespace os_win {

struct DirCacheEntry {
  std::wstring filename;
  FILE_ID_128 id;
};

// The order of these is important.  Must come before UniqueFDTraits struct
// else linker error.  Embedding in struct also causes linker error.

struct UniqueFDTraits {
  static std::mutex file_map_mutex;
  static std::map<HANDLE, DirCacheEntry> file_map;

  static HANDLE InvalidValue() { return INVALID_HANDLE_VALUE; }
  static bool IsValid(HANDLE value) { return value != InvalidValue(); }
  static void Free_Handle(HANDLE fd);

  static void Free(HANDLE fd) {
    RemoveCacheEntry(fd);

    UniqueFDTraits::Free_Handle(fd);
  }

  static void RemoveCacheEntry(HANDLE fd) {
    const std::lock_guard<std::mutex> lock(file_map_mutex);

    file_map.erase(fd);
  }

  static void StoreCacheEntry(HANDLE fd, DirCacheEntry state) {
    const std::lock_guard<std::mutex> lock(file_map_mutex);
    file_map[fd] = state;
  }

  static std::optional<DirCacheEntry> GetCacheEntry(HANDLE fd) {
    const std::lock_guard<std::mutex> lock(file_map_mutex);
    auto found = file_map.find(fd);
    return found == file_map.end()
               ? std::nullopt
               : std::optional<DirCacheEntry>{found->second};
  }
};

}  // namespace os_win

#else  // FOREVER_OS_WIN

namespace os_unix {

struct UniqueFDTraits {
  static int InvalidValue() { return -1; }
  static bool IsValid(int value) { return value >= 0; }
  static void Free(int fd);
};

struct UniqueDirTraits {
  static DIR* InvalidValue() { return nullptr; }
  static bool IsValid(DIR* value) { return value != nullptr; }
  static void Free(DIR* dir);
};

}  // namespace os_unix

#endif  // FOREVER_OS_WIN

}  // namespace INTERNAL

#if FOREVER_OS_WIN

using UniqueFD = UniqueObject<HANDLE, INTERNAL::os_win::UniqueFDTraits>;

#else  // FOREVER_OS_WIN

using UniqueFD = UniqueObject<int, INTERNAL::os_unix::UniqueFDTraits>;
using UniqueDir = UniqueObject<DIR*, INTERNAL::os_unix::UniqueDirTraits>;

#endif  // FOREVER_OS_WIN

}  // namespace FOREVER

#endif  // UNIQUE_FD_H_
