
#ifndef FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_
#define FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_

#include <functional>
#include <string>
#include <utility>
#include "network_monitor.h"
#include "memory/singleton_stdmutex.h"
#include "network_monitor.h"
#include "jni/jni_utils.hpp"
#include "jni/java_method.hpp"
#include "jni/java_class.hpp"
#include "jni/java_global_weak_ref.hpp"
#include "log/log_level.h"
#include "log/log_settings.h"
#include "log/logging.h"

namespace FOREVER {

class JNIAndroidNetworkMonitor : public MEMORY::Singleton<JNIAndroidNetworkMonitor> {
  friend class Singleton<JNIAndroidNetworkMonitor>;
 public:
   explicit JNIAndroidNetworkMonitor();

   ~JNIAndroidNetworkMonitor() noexcept;

   void Init();

   void UnInit();

   void SetNetworkChangeListener(std::function<void(ConnectionType, bool)> callback);

   void NotifyNetworkChange(ConnectionType connectionType, bool is_lan_connected);

   bool IsLanConnected();

   // Returns the type of connection used currently to access the internet
   ConnectionType GetCurrentConnection();

private:
   std::function<void(ConnectionType, bool)> callback_;
   JNI_UTIL::JavaGlobalWeakRef m_android_network_state_tracker_monitor_weak_ref_;
   JNI_UTIL::JavaMethod android_network_state_tracker_monitor_constructor_;
   JNI_UTIL::JavaMethod android_network_state_tracker_monitor_current_state_;
   JNI_UTIL::JavaMethod android_network_state_tracker_monitor_add_listener_;
   JNI_UTIL::JavaMethod android_network_state_tracker_monitor_remove_listener_;
};
}  // namespace FOREVER

#endif  // FOREVER_JNI_ANDROID_NETWORK_MONITOR_H_