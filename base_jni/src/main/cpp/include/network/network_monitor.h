
#ifndef FOREVER_NETWORK_MONITOR_H_
#define FOREVER_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>
#include "connection_type.h"

namespace FOREVER {

class NetworkMonitor {
 public:

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
