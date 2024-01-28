
#ifndef FOREVER_NETWORK_MONITOR_H_
#define FOREVER_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>

namespace FOREVER {

class NetworkMonitor {
 public:
  enum class ConnectionType : int {
    kUnknown = 0,  // A connection exists, but its type is unknown.
                   // Also used as a default value.
    kEthernet = 1,
    kWifi = 2,
    k2G = 3,
    k3G = 4,
    k4G = 5,
    kNone = 6,  // No connection.
    kBluetooth = 7,
    k5G = 8,
    kLast = 9
  };

  explicit NetworkMonitor(std::function<void(ConnectionType, bool)> callback) {
    callback_ = std::move(callback);
  }

  virtual ~NetworkMonitor() = default;

  // Returns true if connected to an AP (Access Point), not necessarily
  // connected to the internet
  virtual bool IsLanConnected() = 0;

  // Returns the type of connection used currently to access the internet
  virtual ConnectionType GetCurrentConnection() = 0;

  virtual void NotifyNetworkChange(ConnectionType connectionType, bool is_lan_connected) = 0;

 protected:
  std::function<void(ConnectionType, bool)> callback_;
};
}  // namespace FOREVER

#endif  // FOREVER_NETWORK_MONITOR_H_
