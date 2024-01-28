
#ifndef FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_
#define FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>
#include "network_monitor.h"
#include "fake_network_monitor.h"
#include "memory/singleton_stdmutex.h"
#include "network_monitor.h"
#include "jni/jni_utils.hpp"
#include "jni/java_method.hpp"
#include "jni/java_class.hpp"
#include "jni/java_global_weak_ref.hpp"

namespace FOREVER {

class JNIAndroidNetworkMonitor : public MEMORY::Singleton<JNIAndroidNetworkMonitor> {
 public:
   explicit JNIAndroidNetworkMonitor();

   ~JNIAndroidNetworkMonitor();

   static JNIAndroidNetworkMonitor* GetInstance();

   void SetNetworkChangeListener(std::function<void(NetworkMonitor::ConnectionType, bool)> callback);

   void NotifyNetworkChange(NetworkMonitor::ConnectionType connectionType, bool is_lan_connected);

   bool IsLanConnected();

   // Returns the type of connection used currently to access the internet
   NetworkMonitor::ConnectionType GetCurrentConnection();

private:
   std::function<void(NetworkMonitor::ConnectionType, bool)> callback_;
   JNI_UTIL::JavaGlobalWeakRef m_android_network_state_tracker_monitor_weak_ref;
};
}  // namespace FOREVER

#endif  // FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_