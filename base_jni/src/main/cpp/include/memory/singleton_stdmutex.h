
#ifndef MEMORY_SINGLETON_STDMUTEX_H_
#define MEMORY_SINGLETON_STDMUTEX_H_

#include <mutex>

#include "macros/macros.h"

namespace FOREVER {
namespace MEMORY {

template <class T>
class Singleton {
 public:
  Singleton() {}
  virtual ~Singleton() {}

  static T* GetInstance() {
    if (once_init_) {
      singleton_mutex_.lock();
      if (once_init_) {
        Init();
        once_init_ = false;
      }
      singleton_mutex_.unlock();
    }
    return instance_;
  }

 private:
  FOREVER_DISALLOW_COPY_AND_ASSIGN(Singleton);
  static void Init() {
    instance_ = new T();
  }

  static T* instance_;  // Leaky singleton.
  static std::mutex singleton_mutex_;
  static bool once_init_;
};

template <class T> T* Singleton<T>::instance_;
template <class T> std::mutex Singleton<T>::singleton_mutex_;
template <class T> bool Singleton<T>::once_init_ = true;

}  // namespace MEMORY
}  // namespace FOREVER

#endif  // MEMORY_SINGLETON_STDMUTEX_H_
