#include "jni/jni_utils.hpp"

#include <memory>
#include <string>

#include "jni/java_class.hpp"
#include "jni/java_method.hpp"

using namespace FOREVER::JNI_UTIL;
using namespace std;

static std::unique_ptr<JniUtils> s_instance;

void JniUtils::initialize(JavaVM* vm, jint vm_version) noexcept {
  // REALM_ASSERT_DEBUG(!s_instance);
  s_instance = std::unique_ptr<JniUtils>(new JniUtils(vm, vm_version));
}

void JniUtils::release() {
  // REALM_ASSERT_DEBUG(s_instance);
  s_instance.release();
}

JNIEnv* JniUtils::get_env(bool attach_if_needed) {
  // REALM_ASSERT_DEBUG(s_instance);

  JNIEnv* env;
  if (s_instance->m_vm->GetEnv(reinterpret_cast<void**>(&env),
                               s_instance->m_vm_version) != JNI_OK) {
    if (attach_if_needed) {
      jint ret = s_instance->m_vm->AttachCurrentThread(&env, nullptr);
      // REALM_ASSERT_RELEASE(ret == JNI_OK);
    } else {
      // REALM_ASSERT_RELEASE(false);
    }
  }

  return env;
}

void JniUtils::detach_current_thread() {
  s_instance->m_vm->DetachCurrentThread();
}

void JniUtils::keep_global_ref(JavaGlobalRefByMove& ref) {
  s_instance->m_global_refs.push_back(std::move(ref));
}

jobject JniUtils::to_hash_map(JNIEnv* env,
                              std::map<std::string, std::string> map) {
  static JavaClass hash_map_class(env, "java/util/HashMap");
  static JavaMethod hash_map_constructor(env, hash_map_class, "<init>", "(I)V");
  static JavaMethod hash_map_put(
      env, hash_map_class, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  jobject hash_map =
      env->NewObject(hash_map_class, hash_map_constructor, (jint)map.size());

  for (const auto& it : map) {
    jstring key = env->NewStringUTF(it.first.c_str());
    jstring value = env->NewStringUTF(it.second.c_str());

    env->CallObjectMethod(hash_map, hash_map_put, key, value);

    env->DeleteLocalRef(key);
    env->DeleteLocalRef(value);
  }

  return hash_map;
}
