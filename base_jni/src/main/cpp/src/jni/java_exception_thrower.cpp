
#include "jni/java_exception_thrower.hpp"

#include <string>

#include "log/log_level.h"
#include "log/log_settings.h"
#include "log/logging.h"
using namespace FOREVER::JNI_UTIL;

const char* JavaExceptionDef::IllegalState = "java/lang/IllegalStateException";
const char* JavaExceptionDef::IllegalArgument = "java/lang/IllegalArgumentException";
const char* JavaExceptionDef::OutOfMemory = "java/lang/OutOfMemoryError";
const char* JavaExceptionDef::NullPointerException = "java/lang/NullPointerException";

JavaExceptionThrower::JavaExceptionThrower(const char* file_path, int line_num)
    : std::runtime_error(
          "Java exception has been occurred. Terminate JNI by throwing a c++ "
          "exception."),
      m_exception_class(),
      m_file_path(file_path),
      m_line_num(line_num) {}

JavaExceptionThrower::JavaExceptionThrower(JNIEnv* env, const char* class_name,
                                           std::string message,
                                           const char* file_path, int line_num)
    : std::runtime_error(std::move(message)),
      m_exception_class(env, class_name, false),
      m_file_path(file_path),
      m_line_num(line_num) {}

void JavaExceptionThrower::throw_java_exception(JNIEnv* env) {
  std::string message = std::string(what()) + "\n" + "(" + m_file_path + ":" +
                        std::to_string(m_line_num) + ")";
  FOREVER_LOG(ERROR) << message.c_str();
  // There is a pending Java exception, just return.
  if (env->ExceptionCheck()) {
    FOREVER_LOG(ERROR) << ("There is a pending Java exception.");
    return;
  }
  env->ThrowNew(m_exception_class, message.c_str());
}

void JavaExceptionThrower::terminate_jni_if_java_exception_occurred(
    JNIEnv* env, CleanUpFunction clean_up_func, const char* file_path,
    int line_num) {
  if (!env->ExceptionCheck()) {
    return;
  }
  if (clean_up_func) {
    clean_up_func();
  }
  throw JavaExceptionThrower(file_path, line_num);
}
