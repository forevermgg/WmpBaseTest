
#ifndef FOREVER_TEST_FAKE_NETWORK_MONITOR_H_
#define FOREVER_TEST_FAKE_NETWORK_MONITOR_H_

#include <functional>

#include "network_monitor.h"

namespace FOREVER {

class FakeNetworkMonitor : public NetworkMonitor {
 public:
  explicit FakeNetworkMonitor(
      std::function<void(NetworkMonitor::ConnectionType, bool)> callback)
      : NetworkMonitor(callback) {}

  ~FakeNetworkMonitor() override { callback_ = nullptr; }

  bool IsLanConnected() override { return is_lan_connected_; }

  NetworkMonitor::ConnectionType GetCurrentConnection() override {
    return NetworkMonitor::ConnectionType::kWifi;
  }

  void SetLanConnected(bool connected) { is_lan_connected_ = connected; }

  void TestNetworkChangeToEthernet() {
    callback_(NetworkMonitor::ConnectionType::kEthernet,
              is_lan_connected_);
  }

 private:
  bool is_lan_connected_ = true;
};

}  // namespace FOREVER

#endif  // FOREVER_TEST_FAKE_NETWORK_MONITOR_H_
