
#ifndef MEMORY_SINGLETON_STDMUTEX_H_
#define MEMORY_SINGLETON_STDMUTEX_H_

#include <atomic>
#include <mutex>
#include "log/log_level.h"
#include "log/logging.h"
#include "log/log_settings.h"
#include "macros/macros.h"

namespace FOREVER {
namespace MEMORY {

template <class T>
class Singleton {
 public:
  friend T;

 public:
  Singleton() {}
  virtual ~Singleton() {}

  static T* GetInstance() {
    T* p = instance_.load(std::memory_order_acquire);
    std::atomic_thread_fence(std::memory_order_acquire);  // 获取内存fence
    if (p == nullptr) {                                   // 1st check
      std::lock_guard<std::mutex> lock(singleton_mutex_);
      p = instance_.load(std::memory_order_acquire);
      if (p == nullptr) {  // 2nd (double) check
        p = new T;
        std::atomic_thread_fence(std::memory_order_release);
        instance_.store(p, std::memory_order_relaxed);
        FOREVER_LOG(ERROR) << "GetSingletonInstance";
        std::atexit(Destroy);
      }
    }
    return p;
  }

  static void Destroy() {
    std::lock_guard<std::mutex> lock(singleton_mutex_);
    T* p = instance_.load(std::memory_order_acquire);
    std::atomic_thread_fence(std::memory_order_acquire);  // 获取内存fence
    if (p == nullptr) {
      FOREVER_LOG(ERROR) << "DestroySingleton But Instance is nullptr";
      std::atomic_thread_fence(std::memory_order_release);
    } else {
      if (instance_ != nullptr) {
        delete instance_.exchange(nullptr, std::memory_order_relaxed);
      }
      std::atomic_thread_fence(std::memory_order_release);
        FOREVER_LOG(ERROR) << "DestroySingleton";
    }
  }

  static bool Inited() {
    std::lock_guard<std::mutex> lock(singleton_mutex_);
    T* p = instance_.load(std::memory_order_acquire);
    std::atomic_thread_fence(std::memory_order_acquire);  // 获取内存fence
    if (p == nullptr) {
      std::atomic_thread_fence(std::memory_order_release);
      return false;
    } else {
      std::atomic_thread_fence(std::memory_order_release);
      return true;
    }
  }

 private:
  FOREVER_DISALLOW_COPY_AND_ASSIGN(Singleton);

  static std::atomic<T*> instance_;
  static std::mutex singleton_mutex_;
};

template <class T>
std::atomic<T*> Singleton<T>::instance_ = nullptr;

template <class T>
std::mutex Singleton<T>::singleton_mutex_;
}  // namespace MEMORY
}  // namespace FOREVER

#endif  // MEMORY_SINGLETON_STDMUTEX_H_
