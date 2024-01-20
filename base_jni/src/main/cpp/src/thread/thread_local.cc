#include "thread/thread_local.h"

#include <cstring>

#include "log/logging.h"

namespace FOREVER {
namespace INTERNAL {

ThreadLocalPointer::ThreadLocalPointer(void (*destroy)(void*)) {
  FOREVER_CHECK(pthread_key_create(&key_, destroy) == 0);
}

ThreadLocalPointer::~ThreadLocalPointer() {
  FOREVER_CHECK(pthread_key_delete(key_) == 0);
}

void* ThreadLocalPointer::get() const {
  return pthread_getspecific(key_);
}

void* ThreadLocalPointer::swap(void* ptr) {
  void* old_ptr = get();
  int err = pthread_setspecific(key_, ptr);
  if (err) {
    FOREVER_CHECK(false) << "pthread_setspecific failed (" << err
                     << "): " << strerror(err);
  }
  return old_ptr;
}

}  // namespace INTERNAL
}  // namespace FOREVER
