
#ifndef UTILS_THREADUTILS_H
#define UTILS_THREADUTILS_H

#include <thread>

namespace FOREVER {

class ThreadUtils {
public:
  static std::thread::id getThreadId() noexcept;
  static bool isThisThread(std::thread::id id) noexcept;
};
} // namespace FOREVER

#endif // UTILS_THREADUTILS_H
