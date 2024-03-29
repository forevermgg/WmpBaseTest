
#include "jni/java_global_weak_ref.hpp"

#include "jni/java_local_ref.hpp"

using namespace FOREVER::JNI_UTIL;

JavaGlobalWeakRef::JavaGlobalWeakRef() : m_weak(nullptr) {}

JavaGlobalWeakRef::JavaGlobalWeakRef(JNIEnv* env, jobject obj)
    : m_weak(obj ? env->NewWeakGlobalRef(obj) : nullptr) {}

JavaGlobalWeakRef::~JavaGlobalWeakRef() {
  if (m_weak) {
    JniUtils::get_env()->DeleteWeakGlobalRef(m_weak);
  }
}

JavaGlobalWeakRef::JavaGlobalWeakRef(JavaGlobalWeakRef&& rhs)
    : m_weak(rhs.m_weak) {
  rhs.m_weak = nullptr;
}

JavaGlobalWeakRef& JavaGlobalWeakRef::operator=(JavaGlobalWeakRef&& rhs) {
  this->~JavaGlobalWeakRef();
  new (this) JavaGlobalWeakRef(std::move(rhs));
  return *this;
}

JavaGlobalWeakRef::JavaGlobalWeakRef(const JavaGlobalWeakRef& rhs)
    : m_weak(JniUtils::get_env(true)->NewWeakGlobalRef(rhs.m_weak)) {}

JavaGlobalWeakRef& JavaGlobalWeakRef::operator=(const JavaGlobalWeakRef& rhs) {
  new (this) JavaGlobalWeakRef(rhs);
  return *this;
}

JavaGlobalRefByMove JavaGlobalWeakRef::global_ref(JNIEnv* env) const {
  if (!env) {
    env = JniUtils::get_env(true);
  }
  return JavaGlobalRefByMove(env, m_weak);
}

bool JavaGlobalWeakRef::call_with_local_ref(
    JNIEnv* env, std::function<Callback> callback) const {
  if (!m_weak) {
    return false;
  }

  JavaLocalRef<jobject> obj(env, m_weak, need_to_create_local_ref);

  if (!obj) {
    return false;
  }
  callback(env, obj);
  return true;
}

bool JavaGlobalWeakRef::call_with_local_ref(
    std::function<Callback> callback) const {
  return call_with_local_ref(JniUtils::get_env(), callback);
}
