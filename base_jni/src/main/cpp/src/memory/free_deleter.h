
#ifndef FOREVER_MEMORY_FREE_DELETER_H_
#define FOREVER_MEMORY_FREE_DELETER_H_

#include <stdlib.h>

namespace FOREVER {

// Function object which invokes 'free' on its parameter, which must be
// a pointer. Can be used to store malloc-allocated pointers in std::unique_ptr:
//
// std::unique_ptr<int, base::FreeDeleter> foo_ptr(
//     static_cast<int*>(malloc(sizeof(int))));
struct FreeDeleter {
  inline void operator()(void* ptr) const {
    free(ptr);
  }
};

}  // namespace FOREVER

#endif  // FOREVER_MEMORY_FREE_DELETER_H_
