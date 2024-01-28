
#ifndef FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_
#define FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>
#include "network_monitor.h"
#include "fake_network_monitor.h"
#include "memory/singleton_stdmutex.h"
#include "network_monitor.h"

namespace FOREVER {

class JNIAndroidNetworkMonitor : public MEMORY::Singleton<JNIAndroidNetworkMonitor> {
 public:
   explicit JNIAndroidNetworkMonitor();

   ~JNIAndroidNetworkMonitor();

   static JNIAndroidNetworkMonitor* GetInstance();

   void SetNetworkChangeListener(std::function<void(NetworkMonitor::ConnectionType, bool)> callback);

   void NotifyNetworkChange(NetworkMonitor::ConnectionType connectionType, bool is_lan_connected);

private:
   std::function<void(NetworkMonitor::ConnectionType, bool)> callback_;
};
}  // namespace FOREVER

#endif  // FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_