#include "network/jni_android_network_monitor.h"

#include <jni.h>

#include "android/android_utils.h"
#include "network/android_network_monitor.h"

namespace FOREVER {

static JNI_UTIL::JavaClass& get_android_network_state_tracker_monitor(
    JNIEnv* env) {
  static JNI_UTIL::JavaClass shared_android_network_state_tracker_monitor_class(
      env, "com/mgg/base/trackers/network/NetworkStateTrackerMonitor");
  return shared_android_network_state_tracker_monitor_class;
}

JNIAndroidNetworkMonitor::JNIAndroidNetworkMonitor() {
  auto env = FOREVER::JNI_UTIL::JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return;
  }

  static JNI_UTIL::JavaMethod android_network_state_tracker_monitor_constructor(
      env, get_android_network_state_tracker_monitor(env), "init",
      "(Landroid/app/Application;)V", false);
  android_network_state_tracker_monitor_constructor_ =
      android_network_state_tracker_monitor_constructor;

  JNI_UTIL::JavaMethod android_network_state_tracker_monitor_current_state(
      env, get_android_network_state_tracker_monitor(env), "isLanConnected",
      "()Z");
  android_network_state_tracker_monitor_current_state_ =
      android_network_state_tracker_monitor_current_state;

  JNI_UTIL::JavaMethod android_network_state_tracker_monitor_add_listener(
      env, get_android_network_state_tracker_monitor(env), "isLanConnected",
      "()Z");
  android_network_state_tracker_monitor_add_listener_ =
      android_network_state_tracker_monitor_add_listener;

  JNI_UTIL::JavaMethod android_network_state_tracker_monitor_remove_listener(
      env, get_android_network_state_tracker_monitor(env), "isLanConnected",
      "()Z");
  android_network_state_tracker_monitor_remove_listener_ =
      android_network_state_tracker_monitor_remove_listener;

  if (!m_android_network_state_tracker_monitor_weak_ref_) {
    jobject application = nullptr;
    jclass activity_thread_clz = env->FindClass("android/app/ActivityThread");
    if (activity_thread_clz != nullptr) {
      jmethodID currentApplication =
          env->GetStaticMethodID(activity_thread_clz, "currentApplication",
                                 "()Landroid/app/Application;");
      if (currentApplication != nullptr) {
        application = env->CallStaticObjectMethod(activity_thread_clz,
                                                  currentApplication);
      } else {
        printf("Cannot find method: currentApplication() in ActivityThread.");
      }
      env->DeleteLocalRef(activity_thread_clz);
    } else {
      printf("Cannot find class: android.app.ActivityThread");
    }
    jobject android_network_state_tracker_monitor_object = env->NewObject(
        get_android_network_state_tracker_monitor(env),
        android_network_state_tracker_monitor_constructor_, application);
    m_android_network_state_tracker_monitor_weak_ref_ =
        JNI_UTIL::JavaGlobalWeakRef(
            env, android_network_state_tracker_monitor_object);
  }
  if (m_android_network_state_tracker_monitor_weak_ref_ &&
      android_network_state_tracker_monitor_add_listener_) {
    m_android_network_state_tracker_monitor_weak_ref_.call_with_local_ref(
        env, [&](JNIEnv*, jobject obj) {
          env->CallVoidMethod(
              obj, android_network_state_tracker_monitor_add_listener_);
        });
  }
}

JNIAndroidNetworkMonitor::~JNIAndroidNetworkMonitor() noexcept {
  auto env = FOREVER::JNI_UTIL::JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return;
  }
  if (m_android_network_state_tracker_monitor_weak_ref_ &&
      android_network_state_tracker_monitor_remove_listener_) {
    m_android_network_state_tracker_monitor_weak_ref_.call_with_local_ref(
        env, [&](JNIEnv*, jobject obj) {
          env->CallVoidMethod(
              obj, android_network_state_tracker_monitor_remove_listener_);
        });
  }
}
// static
JNIAndroidNetworkMonitor* JNIAndroidNetworkMonitor::GetInstance() {
  return Singleton<JNIAndroidNetworkMonitor>::GetInstance();
}

void JNIAndroidNetworkMonitor::SetNetworkChangeListener(
    std::function<void(NetworkMonitor::ConnectionType, bool)> callback) {
  this->callback_ = callback;
}

void JNIAndroidNetworkMonitor::NotifyNetworkChange(
    NetworkMonitor::ConnectionType connectionType, bool is_lan_connected) {
  if (callback_ != nullptr) {
    callback_(connectionType, is_lan_connected);
  }
}

bool JNIAndroidNetworkMonitor::IsLanConnected() {
  auto env = FOREVER::JNI_UTIL::JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return false;
  }
  if (m_android_network_state_tracker_monitor_weak_ref_ &&
      android_network_state_tracker_monitor_current_state_) {
    m_android_network_state_tracker_monitor_weak_ref_.call_with_local_ref(
        env, [&](JNIEnv*, jobject obj) {
          return env->CallBooleanMethod(
              obj, android_network_state_tracker_monitor_current_state_);
        });
  }
  return false;
}

NetworkMonitor::ConnectionType
JNIAndroidNetworkMonitor::GetCurrentConnection() {
  auto env = FOREVER::JNI_UTIL::JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return NetworkMonitor::ConnectionType::kUnknown;
  }
  return NetworkMonitor::ConnectionType::kUnknown;
}
}  // namespace FOREVER

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_base_trackers_network_NetworkStateTrackerMonitor_onConstraintChanged(
    JNIEnv* env, jobject thiz, jint connection_type, jboolean is_connected) {
  FOREVER::JNIAndroidNetworkMonitor::GetInstance()->NotifyNetworkChange(
      FOREVER::NetworkMonitor::ConnectionType::kUnknown, is_connected);
}