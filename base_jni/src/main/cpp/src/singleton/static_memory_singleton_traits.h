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

template <typename Type>
struct StaticMemorySingletonTraits {
  // WARNING: User has to support a New() which returns null.
  static Type* New() {
    // Only constructs once and returns pointer; otherwise returns null.
    if (dead_.exchange(true, std::memory_order_relaxed)) return nullptr;
    return new (buffer_) Type();
  }

  static void Delete(Type* p) {
    if (p) p->Type::~Type();
  }

  static const bool kRegisterAtExit = true;

 private:
  alignas(Type) static char buffer_[sizeof(Type)];
  // Signal the object was already deleted, so it is not revived.
  static std::atomic<bool> dead_;
};

template <typename Type>
alignas(Type) char StaticMemorySingletonTraits<Type>::buffer_[sizeof(Type)];
template <typename Type>
std::atomic<bool> StaticMemorySingletonTraits<Type>::dead_ = false;
}  // namespace MEMORY
}  // namespace FOREVER

#endif  // MEMORY_SINGLETON_H_