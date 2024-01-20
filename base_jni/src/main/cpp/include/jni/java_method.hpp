#ifndef FOREVER_UTIL_JAVA_METHOD_HPP
#define FOREVER_UTIL_JAVA_METHOD_HPP

#include <jni.h>

namespace FOREVER {
namespace JNI_UTIL {

class JavaClass;
// RAII wrapper for java method ID. Since normally method ID stays unchanged for
// the whole JVM life cycle, it would be safe to have a static JavaMethod object
// to avoid calling GetMethodID multiple times.
class JavaMethod {
 public:
  JavaMethod() : m_method_id(nullptr) {}
  // Lookup a method on a named class
  JavaMethod(JNIEnv* env, JavaClass const& cls, const char* method_name,
             const char* signature, bool static_method = false);

  // Lookup a method on a object from Java
  JavaMethod(JNIEnv* env, jobject const& cls, const char* method_name,
             const char* signature);

  // From https://developer.android.com/training/articles/perf-jni.html
  // The class references, field IDs, and method IDs are guaranteed valid until
  // the class is unloaded. Classes are only unloaded if all classes associated
  // with a ClassLoader can be garbage collected, which is rare but will not be
  // impossible in Android. Note however that the jclass is a class reference
  // and must be protected with a call to NewGlobalRef (see the next section).
  //
  // BUT THERE ARE BUGS. See below:
  //
  // WARNING!! For anyone wants to implement this method, please DON'T. There
  // might be a bug in JVM implementation that the jmethodID retrieved from
  // jobject's jclass would be invalid under some certain conditions. See
  // https://github.com/FOREVER/FOREVER-java/issues/4964 for how to reproduce
  // it. JavaMethod(JNIEnv* env, jobject obj, const char* method_name, const
  // char* signature);

  // For this constructor, there is no evidence that jmethodID will be
  // invalidated when there is no ref to the jclass. Just in case, though we
  // force the caller to keep a global ref by JavaClass to make sure we won't
  // encounter another JVM bug. JavaMethod(JNIEnv* env, const char* class_name,
  // const char* method_name, const char* signature,
  //           bool static_method = false);

  JavaMethod(const JavaMethod&) = default;
  JavaMethod& operator=(const JavaMethod&) = default;
  JavaMethod(JavaMethod&& rhs) = delete;
  JavaMethod& operator=(JavaMethod&& rhs) = delete;

  ~JavaMethod() {}

  inline operator bool() const noexcept { return m_method_id != nullptr; }
  inline operator const jmethodID&() const noexcept { return m_method_id; }

 private:
  jmethodID m_method_id;
};

}  // namespace JNI_UTIL
}  // namespace FOREVER

#endif  // FOREVER_UTIL_JAVA_METHOD_HPP
