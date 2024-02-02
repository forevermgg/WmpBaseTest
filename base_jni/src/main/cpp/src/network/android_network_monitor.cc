#include "network/android_network_monitor.h"

#include <jni.h>

#include "network/jni_android_network_monitor.h"

namespace FOREVER {
bool AndroidNetworkMonitor::InitJNICallBack() {
  JNIAndroidNetworkMonitor::GetInstance()->Init();
  JNIAndroidNetworkMonitor::GetInstance()->SetNetworkChangeListener(
      [this](NetworkMonitor::ConnectionType connection_type,
             bool is_lan_connected) {
        NotifyNetworkChange(connection_type, is_lan_connected);
      });
  return true;
}

bool AndroidNetworkMonitor::UnInitJNICallBack() {
  JNIAndroidNetworkMonitor::GetInstance()->SetNetworkChangeListener(nullptr);
  JNIAndroidNetworkMonitor::GetInstance()->UnInit();
  return true;
}

bool AndroidNetworkMonitor::IsLanConnected() {
  JNIAndroidNetworkMonitor::GetInstance()->IsLanConnected();
  return true;
}

NetworkMonitor::ConnectionType AndroidNetworkMonitor::GetCurrentConnection() {
  return NetworkMonitor::ConnectionType::kEthernet;
}

void AndroidNetworkMonitor::NotifyNetworkChange(
    NetworkMonitor::ConnectionType connectionType, bool is_lan_connected) {
  if (callback_) {
    callback_(connectionType, is_lan_connected);
  }
}

}  // namespace FOREVER