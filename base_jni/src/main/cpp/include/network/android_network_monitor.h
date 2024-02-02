
#ifndef FOREVER_ANDROID_NETWORK_MONITOR_H_
#define FOREVER_ANDROID_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>
#include "network_monitor.h"
#include "log/log_level.h"
#include "log/log_settings.h"
#include "log/logging.h"

namespace FOREVER {

class AndroidNetworkMonitor : public NetworkMonitor {
 public:
   explicit AndroidNetworkMonitor(
            std::function<void(ConnectionType, bool)> callback)
   : NetworkMonitor(callback) {
     InitJNICallBack();
     FOREVER_LOG(ERROR) << "AndroidNetworkMonitor";
   }

   ~AndroidNetworkMonitor() override {
     callback_ = nullptr;
     UnInitJNICallBack();
     FOREVER_LOG(ERROR) << "~AndroidNetworkMonitor";
   }

   bool InitJNICallBack();

   bool UnInitJNICallBack();

   // Returns true if connected to an AP (Access Point), not necessarily
   // connected to the internet
   bool IsLanConnected() override;

   // Returns the type of connection used currently to access the internet
   ConnectionType GetCurrentConnection() override;

   void NotifyNetworkChange(ConnectionType connectionType, bool is_lan_connected) override;

 protected:
};
}  // namespace FOREVER

#endif  // FOREVER_ANDROID_NETWORK_MONITOR_H_