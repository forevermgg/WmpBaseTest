
#ifndef FOREVER_UTIL_JAVA_EXCEPTION_THROWER_HPP
#define FOREVER_UTIL_JAVA_EXCEPTION_THROWER_HPP

#include <jni.h>

#include <functional>
#include <stdexcept>

#include "java_class.hpp"

namespace FOREVER {
namespace JNI_UTIL {

#define THROW_JAVA_EXCEPTION(env, class_name, message)                    \
  throw FOREVER::JNI_UTIL::JavaExceptionThrower(env, class_name, message, \
                                                __FILE__, __LINE__)

#define TERMINATE_JNI_IF_JAVA_EXCEPTION_OCCURRED(env, clean_up_func) \
  JavaExceptionThrower::terminate_jni_if_java_exception_occurred(    \
      env, clean_up_func, __FILE__, __LINE__);

// Class to help throw a Java exception from JNI code.
// This exception will be called from CATCH_STD and throw a Java exception
// there.
class JavaExceptionThrower : public std::runtime_error {
 public:
  using CleanUpFunction = std::function<void()>;

  JavaExceptionThrower(const char* file_path, int line_num);
  JavaExceptionThrower(JNIEnv* env, const char* class_name, std::string message,
                       const char* file_path, int line_num);

  void throw_java_exception(JNIEnv* env);

  // This method will throw a JavaExceptionThrower to terminate JNI then return
  // to java if there is an Java exception has been thrown before. clean_up_fucn
  // will be called before throwing the c++ exception if there is a pending java
  // exception.
  static void terminate_jni_if_java_exception_occurred(
      JNIEnv* env, CleanUpFunction clean_up_func, const char* file_path,
      int line_num);

 private:
  JavaClass m_exception_class;
  const char* m_file_path;
  int m_line_num;
};

class JavaExceptionDef {
 public:
  // Class names
  static const char* IllegalState;
  static const char* IllegalArgument;
  static const char* OutOfMemory;
  static const char* NullPointerException;
};
}  // namespace JNI_UTIL
}  // namespace FOREVER

#endif  // FOREVER_UTIL_JAVA_EXCEPTION_THROWER_HPP
