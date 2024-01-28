#include <jni.h>
#include <string>
#include "network/network_sharing_platform.h"
#include "network/android_network_sharing_platform.h"
#include "network/context_impl.h"
#include "network/network_monitor.h"
#include <memory>

std::unique_ptr<FOREVER::Context> context_;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mgg_base_NativeBaseLib_getVersion(JNIEnv *env, jobject thiz) {
    std::string hello = "Base JNI from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_base_NativeBaseLib_register(JNIEnv *env, jobject thiz) {
  auto sharing_platform = FOREVER::AndroidNetWorkSharingPlatform();
  context_ = std::make_unique<FOREVER::ContextImpl>((sharing_platform));
  std::function<void(FOREVER::NetworkMonitor::ConnectionType, bool)> call_back =
          [](FOREVER::NetworkMonitor::ConnectionType connection_type, bool is_lan_connected) {
  };
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_base_NativeBaseLib_unregister(JNIEnv *env, jobject thiz) {
  context_.reset();
}