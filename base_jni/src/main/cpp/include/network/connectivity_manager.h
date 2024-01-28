#ifndef FOREVER_CONNECTIVITY_MANAGER_H_
#define FOREVER_CONNECTIVITY_MANAGER_H_

#include <functional>
#include <string>

namespace FOREVER {

class ConnectivityManager {
 public:
  enum class ConnectionType {
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

  virtual ~ConnectivityManager() = default;

  virtual bool IsLanConnected() = 0;

  virtual ConnectionType GetConnectionType() = 0;

  virtual void RegisterConnectionListener(
      std::string listener_name,
      std::function<void(ConnectionType, bool)>) = 0;
  virtual void UnregisterConnectionListener(
      std::string listener_name) = 0;
};

}  // namespace FOREVER

#endif  // FOREVER_CONNECTIVITY_MANAGER_H_
