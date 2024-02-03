#include <jni.h>
#include <cstdlib>
#include "log/log_settings.h"
#include "log/log_level.h"
#include "log/logging.h"
#include <android/log.h>
#include "jni/jni_utils.hpp"
#include "jni/jni_util.h"
#include "jni/java_class_global_def.hpp"
#include "src/stacktrace/stacktrace.h"
#include "memory/singleton_stdmutex.h"
#include "network/jni_android_network_monitor.h"

void atExitHandler() {
  FOREVER::STACKTRACE::StackTrace stacktrace;
  FOREVER_LOG(ERROR) <<  "Dumping stack:\n" + stacktrace.GetSymbolString() + "\n";
}

// This is called by the VM when the shared library is first loaded.
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  FOREVER::LogSettings log_settings;
  log_settings.min_log_level = FOREVER::kLogInfo;
  FOREVER::SetLogSettings(log_settings);
  FOREVER_LOG(ERROR) << "JNI_OnLoad";
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
     return JNI_ERR;
  } else {
    FOREVER::JNI::InitJavaVM(vm);
    FOREVER::JNI_UTIL::JniUtils::initialize(vm, JNI_VERSION_1_6);
    FOREVER::_IMPL::JavaClassGlobalDef::initialize(env);
  }
  // std::atexit(atExitHandler);
  return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNI_OnUnload(JavaVM* vm, void*) {
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    return;
  } else {
    FOREVER::JNI::Release();
    FOREVER::JNI_UTIL::JniUtils::release();
    FOREVER::_IMPL::JavaClassGlobalDef::release();
    if (FOREVER::JNIAndroidNetworkMonitor::GetInstance()->Inited()) {
      FOREVER::JNIAndroidNetworkMonitor::GetInstance()->Destroy();
    }
  }
}
