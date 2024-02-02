#ifndef FOREVER_CONNECTIVITY_MANAGER_H_
#define FOREVER_CONNECTIVITY_MANAGER_H_

#include <functional>
#include <string>
#include "connection_type.h"

namespace FOREVER {

class ConnectivityManager {
 public:

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
