#include "network/android_network_monitor.h"

namespace FOREVER {

bool AndroidNetworkMonitor::InitJNICall() {
  return true;
}

bool AndroidNetworkMonitor::UnInitJNICall() {
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

} // namespace FOREVER