
#ifndef FOREVER_CONNECTIVITY_MANAGER_IMPL_H_
#define FOREVER_CONNECTIVITY_MANAGER_IMPL_H_

#include <functional>
#include <memory>
#include <string>
#include <map>
#include "connectivity_manager.h"
#include "network_monitor.h"
#include "network_sharing_platform.h"

namespace FOREVER {

// Limitation: this is not thread safe and needs to be enhanced
class ConnectivityManagerImpl : public ConnectivityManager {
 public:
  explicit ConnectivityManagerImpl(FOREVER::SHARING::NetWorkSharingPlatform& platform);

  bool IsLanConnected() override;

  ConnectionType GetConnectionType() override;

  void RegisterConnectionListener(
      std::string listener_name,
      std::function<void(ConnectionType, bool)> callback) override;
  void UnregisterConnectionListener(std::string listener_name) override;

  int GetListenerCount() const;

 private:
  std::map<std::string, std::function<void(ConnectionType, bool)>> listeners_;
  std::unique_ptr<NetworkMonitor> network_monitor_;
};

}  // namespace FOREVER

#endif  // FOREVER_CONNECTIVITY_MANAGER_IMPL_H_
