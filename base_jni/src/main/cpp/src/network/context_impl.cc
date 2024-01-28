#include "network/context_impl.h"

#include <stdint.h>

#include <functional>
#include <memory>
#include <utility>

#include "network/connectivity_manager.h"
#include "network/connectivity_manager_impl.h"

namespace FOREVER {
using FOREVER::SHARING::NetWorkSharingPlatform;
ContextImpl::ContextImpl(NetWorkSharingPlatform& platform)
    : platform_(platform),
      connectivity_manager_(
          std::make_unique<ConnectivityManagerImpl>(platform_)) {}

ConnectivityManager* ContextImpl::GetConnectivityManager() const {
  return connectivity_manager_.get();
}

}  // namespace FOREVER
