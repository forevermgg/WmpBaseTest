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
  if (!m_android_network_state_tracker_monitor_weak_ref) {
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
        android_network_state_tracker_monitor_constructor, application);
    m_android_network_state_tracker_monitor_weak_ref =
        JNI_UTIL::JavaGlobalWeakRef(
            env, android_network_state_tracker_monitor_object);
  }
}

JNIAndroidNetworkMonitor::~JNIAndroidNetworkMonitor() noexcept {}
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
  // 实现连接到AP的逻辑
  // ...
  return true;
}

NetworkMonitor::ConnectionType
JNIAndroidNetworkMonitor::GetCurrentConnection() {
  // 实现获取当前网络连接类型的逻辑
  // ...
  return NetworkMonitor::ConnectionType::kEthernet;
}
}  // namespace FOREVER

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_base_trackers_network_NetworkStateTrackerMonitor_onConstraintChanged(
    JNIEnv* env, jobject thiz, jint connection_type, jboolean is_connected) {
  FOREVER::JNIAndroidNetworkMonitor::GetInstance()->NotifyNetworkChange(
      FOREVER::NetworkMonitor::ConnectionType::kUnknown, is_connected);
}