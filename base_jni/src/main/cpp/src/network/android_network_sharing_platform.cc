#include "network/android_network_sharing_platform.h"

namespace FOREVER {
AndroidNetWorkSharingPlatform::AndroidNetWorkSharingPlatform() {}

AndroidNetWorkSharingPlatform::~AndroidNetWorkSharingPlatform() {}

std::unique_ptr<NetworkMonitor>
AndroidNetWorkSharingPlatform::CreateNetworkMonitor(
    std::function<void(FOREVER::ConnectionType, bool)>
        callback) {
  return std::make_unique<AndroidNetworkMonitor>(std::move(callback));
}

}  // namespace FOREVER