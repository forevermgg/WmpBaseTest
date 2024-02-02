
#include "network/connectivity_manager_impl.h"

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "log/log_level.h"
#include "log/log_settings.h"
#include "log/logging.h"
#include "network/connectivity_manager.h"

namespace FOREVER {
namespace {
using FOREVER::SHARING::NetWorkSharingPlatform;

std::string GetConnectionTypeString(ConnectionType connection_type) {
  switch (connection_type) {
    case ConnectionType::k2G:
      return "2G";
    case ConnectionType::k3G:
      return "3G";
    case ConnectionType::k4G:
      return "4G";
    case ConnectionType::k5G:
      return "5G";
    case ConnectionType::kBluetooth:
      return "Bluetooth";
    case ConnectionType::kEthernet:
      return "Ethernet";
    case ConnectionType::kWifi:
      return "WiFi";
    default:
      return "Unknown";
  }
}

}  // namespace

ConnectivityManagerImpl::ConnectivityManagerImpl(
    NetWorkSharingPlatform& platform) {
  network_monitor_ = platform.CreateNetworkMonitor(
      [this](ConnectionType connection_type,
             bool is_lan_connected) {
        ConnectionType new_connection_type =
            static_cast<ConnectionType>(connection_type);
        FOREVER_LOG(ERROR) << ": New connection type:"
                           << GetConnectionTypeString(new_connection_type);
        for (auto& listener : listeners_) {
          listener.second(new_connection_type, is_lan_connected);
        }
      });
}

bool ConnectivityManagerImpl::IsLanConnected() {
  return network_monitor_->IsLanConnected();
}

ConnectionType ConnectivityManagerImpl::GetConnectionType() {
  return static_cast<ConnectionType>(network_monitor_->GetCurrentConnection());
}

void ConnectivityManagerImpl::RegisterConnectionListener(
    std::string listener_name,
    std::function<void(ConnectionType, bool)> callback) {
  listeners_.emplace(listener_name, std::move(callback));
}

void ConnectivityManagerImpl::UnregisterConnectionListener(
    std::string listener_name) {
  listeners_.erase(listener_name);
}

int ConnectivityManagerImpl::GetListenerCount() const {
  return listeners_.size();
}

}  // namespace FOREVER
