#include "network/android_network_monitor.h"
#include <jni.h>
#include "network/jni_android_network_monitor.h"

namespace FOREVER {

JNIAndroidNetworkMonitor::JNIAndroidNetworkMonitor() {
}

JNIAndroidNetworkMonitor::~JNIAndroidNetworkMonitor() noexcept {

}
// static
JNIAndroidNetworkMonitor* JNIAndroidNetworkMonitor::GetInstance() {
  return Singleton<JNIAndroidNetworkMonitor>::GetInstance();
}

void JNIAndroidNetworkMonitor::SetNetworkChangeListener(
        std::function<void(NetworkMonitor::ConnectionType, bool)> callback) {
  this->callback_ = callback;
}

void JNIAndroidNetworkMonitor::NotifyNetworkChange(NetworkMonitor::ConnectionType connectionType, bool is_lan_connected) {
  if (callback_ != nullptr) {
    callback_(connectionType, is_lan_connected);
  }
}
} // namespace FOREVER


extern "C" JNIEXPORT void JNICALL
Java_com_mgg_base_trackers_network_NetworkStateTrackerMonitor_onConstraintChanged(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jint connection_type,
                                                                                  jboolean is_connected) {

  FOREVER::JNIAndroidNetworkMonitor::GetInstance()->NotifyNetworkChange(
          FOREVER::NetworkMonitor::ConnectionType::kUnknown, is_connected);
}