
#ifndef FOREVER_NETWORK_SHARING_PLATFORM_H_
#define FOREVER_NETWORK_SHARING_PLATFORM_H_

#include <functional>
#include <memory>
#include "network_monitor.h"

namespace FOREVER::SHARING {

// Platform abstraction interface for NearbyShare cross-platform compatibility.
class NetWorkSharingPlatform {
 public:
  virtual ~NetWorkSharingPlatform() = default;

  virtual std::unique_ptr<FOREVER::NetworkMonitor> CreateNetworkMonitor(
      std::function<void(FOREVER::ConnectionType, bool)>
      callback) = 0;
};
}  // namespace FOREVER::SHARING::API

#endif  // FOREVER_NETWORK_SHARING_PLATFORM_H_
