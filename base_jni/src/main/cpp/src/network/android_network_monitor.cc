#include "network/android_network_monitor.h"

#include <jni.h>

#include "network/jni_android_network_monitor.h"

namespace FOREVER {
bool AndroidNetworkMonitor::InitJNICallBack() {
  JNIAndroidNetworkMonitor::GetInstance()->SetNetworkChangeListener(
      [this](NetworkMonitor::ConnectionType connection_type,
             bool is_lan_connected) {
        NotifyNetworkChange(connection_type, is_lan_connected);
      });
  return true;
}

bool AndroidNetworkMonitor::UnInitJNICallBack() {
  JNIAndroidNetworkMonitor::GetInstance()->SetNetworkChangeListener(nullptr);
  return true;
}

bool AndroidNetworkMonitor::IsLanConnected() {
  // 实现连接到AP的逻辑
  // ...
  return true;
}

NetworkMonitor::ConnectionType AndroidNetworkMonitor::GetCurrentConnection() {
  // 实现获取当前网络连接类型的逻辑
  // ...
  return NetworkMonitor::ConnectionType::kEthernet;
}

void AndroidNetworkMonitor::NotifyNetworkChange(
    NetworkMonitor::ConnectionType connectionType, bool is_lan_connected) {
  callback_(connectionType, is_lan_connected);
}

}  // namespace FOREVER