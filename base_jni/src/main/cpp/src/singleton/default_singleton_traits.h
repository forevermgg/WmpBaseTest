#ifndef MEMORY_SINGLETON_H_
#define MEMORY_SINGLETON_H_

#include <atomic>
#include <mutex>

namespace FOREVER {
namespace MEMORY {

template <typename Type>
struct DefaultSingletonTraits {
  // Allocates the object.
  static Type* New() {
    // The parenthesis is very important here; it forces POD type
    // initialization.
    return new Type();
  }

  // Destroys the object.
  static void Delete(Type* x) { delete x; }

  // Set to true to automatically register deletion of the object on process
  // exit. See below for the required call that makes this happen.
  static const bool kRegisterAtExit = true;
};
}  // namespace MEMORY
}  // namespace FOREVER

#endif  // MEMORY_SINGLETON_H_