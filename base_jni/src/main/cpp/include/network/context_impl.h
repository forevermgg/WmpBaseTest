
#ifndef FOREVER_CONTEXT_IMPL_H_
#define FOREVER_CONTEXT_IMPL_H_

#include <stdint.h>

#include <functional>
#include <memory>

#include "connectivity_manager.h"
#include "context.h"
#include "network_sharing_platform.h"

namespace FOREVER {

class ContextImpl : public Context {
 public:
  explicit ContextImpl(FOREVER::SHARING::NetWorkSharingPlatform& platform);
  ~ContextImpl() override = default;
  ConnectivityManager* GetConnectivityManager() const override;

 private:
  FOREVER::SHARING::NetWorkSharingPlatform& platform_;
  std::unique_ptr<ConnectivityManager> connectivity_manager_;
};

}  // namespace FOREVER

#endif  // FOREVER_CONTEXT_IMPL_H_
