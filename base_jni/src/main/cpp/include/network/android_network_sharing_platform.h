
#ifndef FOREVER_ANDROID_NETWORK_SHARING_PLATFORM_H_
#define FOREVER_ANDROID_NETWORK_SHARING_PLATFORM_H_

#include <functional>
#include <memory>
#include "network_sharing_platform.h"
#include "android_network_monitor.h"

namespace FOREVER {
class AndroidNetWorkSharingPlatform : public SHARING::NetWorkSharingPlatform {
 public:
   AndroidNetWorkSharingPlatform();
   ~AndroidNetWorkSharingPlatform() override;

   std::unique_ptr<NetworkMonitor> CreateNetworkMonitor(
           std::function<void(FOREVER::ConnectionType, bool)> callback) override;
   };
}  // namespace FOREVER

#endif  // FOREVER_ANDROID_NETWORK_SHARING_PLATFORM_H_
