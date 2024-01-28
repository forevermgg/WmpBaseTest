
#include "utils/unique_fd.h"

#include "utils//eintr_wrapper.h"

namespace FOREVER {
namespace INTERNAL {

#if FOREVER_OS_WIN

namespace os_win {

std::mutex UniqueFDTraits::file_map_mutex;
std::map<HANDLE, DirCacheEntry> UniqueFDTraits::file_map;

void UniqueFDTraits::Free_Handle(HANDLE fd) {
  CloseHandle(fd);
}

}  // namespace os_win

#else  // FOREVER_OS_WIN

namespace os_unix {

void UniqueFDTraits::Free(int fd) {
  close(fd);
}

void UniqueDirTraits::Free(DIR* dir) {
  closedir(dir);
}

}  // namespace os_unix

#endif  // FOREVER_OS_WIN

}  // namespace INTERNAL
}  // namespace FOREVER
