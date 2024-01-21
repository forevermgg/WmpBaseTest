
#ifndef FOREVER_ANDROID_NETWORK_MONITOR_H_
#define FOREVER_ANDROID_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>
#include "network_monitor.h"
#include "fake_network_monitor.h"

namespace FOREVER {

class AndroidNetworkMonitor : public NetworkMonitor {
 public:
   explicit AndroidNetworkMonitor(
            std::function<void(NetworkMonitor::ConnectionType, bool)> callback)
   : NetworkMonitor(callback) {
     InitJNICall();
   }

   ~AndroidNetworkMonitor() override {
     callback_ = nullptr;
     UnInitJNICall();
   }

   bool InitJNICall();

   bool UnInitJNICall();

   // Returns true if connected to an AP (Access Point), not necessarily
   // connected to the internet
   bool IsLanConnected() override;

   // Returns the type of connection used currently to access the internet
   ConnectionType GetCurrentConnection() override;

 protected:
   std::function<void(ConnectionType, bool)> callback_;
};
}  // namespace FOREVER

#endif  // FOREVER_ANDROID_NETWORK_MONITOR_H_