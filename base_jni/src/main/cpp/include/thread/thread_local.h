#ifndef FOREVER_THREAD_LOCAL_H_
#define FOREVER_THREAD_LOCAL_H_

#include <memory>

#include "macros/macros.h"

#include <pthread.h>

namespace FOREVER {

#define FOREVER_THREAD_LOCAL static

namespace INTERNAL {

class ThreadLocalPointer {
 public:
  explicit ThreadLocalPointer(void (*destroy)(void*));
  ~ThreadLocalPointer();

  void* get() const;
  void* swap(void* ptr);

 private:
  pthread_key_t key_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(ThreadLocalPointer);
};

}  // namespace INTERNAL

template <typename T>
class ThreadLocalUniquePtr {
 public:
  ThreadLocalUniquePtr() : ptr_(destroy) {}

  T* get() const { return reinterpret_cast<T*>(ptr_.get()); }
  void reset(T* ptr) { destroy(ptr_.swap(ptr)); }

 private:
  static void destroy(void* ptr) { delete reinterpret_cast<T*>(ptr); }

  INTERNAL::ThreadLocalPointer ptr_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(ThreadLocalUniquePtr);
};


#define FOREVER_THREAD_LOCAL static thread_local

template <typename T>
class ThreadLocalUniquePtr2 {
 public:
  ThreadLocalUniquePtr2() = default;

  T* get() const { return ptr_.get(); }
  void reset(T* ptr) { ptr_.reset(ptr); }

 private:
  std::unique_ptr<T> ptr_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(ThreadLocalUniquePtr2);
};
}  // namespace FOREVER

#endif  // FOREVER_THREAD_LOCAL_H_
