
#include "android/android_utils.h"

#include <android/log.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <jni.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h> /* for fdstat() */
#include <sys/system_properties.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>
#include <string>

#define LOG_ERR(...) \
  __android_log_print(ANDROID_LOG_ERROR, "ANDROID_UTILS", __VA_ARGS__);

#define LOG_WARN(...) \
  __android_log_print(ANDROID_LOG_WARN, "ANDROID_UTILS", __VA_ARGS__);

#define GET_PROP(name, trans)                   \
  do {                                          \
    char _v[PROP_VALUE_MAX] = {0};              \
    if (__system_property_get(name, _v) == 0) { \
      return false;                             \
    }                                           \
    trans;                                      \
  } while (0)

#define GET_STRING_PROP(n, t) GET_PROP(n, t = _v)
#define GET_INT_PROP(n, t) GET_PROP(n, t = atoi(_v))
#define GET_STRING_LIST_PROP(n, t)      \
  do {                                  \
    std::string _l, _t;                 \
    GET_STRING_PROP(n, _l);             \
    std::istringstream _s(_l);          \
    while (std::getline(_s, _t, ',')) { \
      t.push_back(_t);                  \
    }                                   \
  } while (0)

namespace FOREVER {
uint32_t sApiLevel = 0;
std::once_flag sApiLevelOnceFlag;

static int api_level() {
  std::call_once(sApiLevelOnceFlag, []() {
    char sdkVersion[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", sdkVersion);
    sApiLevel = atoi(sdkVersion);
  });
  return sApiLevel;
}

static std::string GetAndroidProp(const char* name) {
  std::string ret;
  // int api_level = android_get_device_api_level();
#if __ANDROID_API__ >= 26
  const prop_info* pi = __system_property_find(name);
  if (!pi) {
    return ret;
  }
  __system_property_read_callback(
      pi,
      [](void* dst_void, const char*, const char* value, uint32_t) {
        std::string& dst = *static_cast<std::string*>(dst_void);
        dst = value;
      },
      &ret);
#else  // __ANDROID_API__ < 26
  char value_buf[PROP_VALUE_MAX];
  int len = __system_property_get(name, value_buf);
  if (len > 0 && static_cast<size_t>(len) < sizeof(value_buf)) {
    ret = std::string(value_buf, static_cast<size_t>(len));
  }
#endif
  return ret;
}

/* Weak symbol import */
void __system_property_read_callback(const prop_info* info,
                                     void (*callback)(void* cookie,
                                                      const char* name,
                                                      const char* value,
                                                      uint32_t serial),
                                     void* cookie) __attribute__((weak));

/* Callback used with __system_property_read_callback. */
static void prop_read_int(void* cookie, const char* name, const char* value,
                          uint32_t serial) {
  *(int*)cookie = atoi(value);
  (void)name;
  (void)serial;
}

static int system_property_get_int(const char* name) {
  int result = 0;
  if (__system_property_read_callback) {
    const prop_info* info = __system_property_find(name);
    if (info) __system_property_read_callback(info, &prop_read_int, &result);
  } else {
    char value[PROP_VALUE_MAX] = {};
    if (__system_property_get(name, value) >= 1) result = atoi(value);
  }
  return result;
}

static int device_api_level() {
  static int s_api_level = -1;
  if (s_api_level < 0)
    s_api_level = system_property_get_int("ro.build.version.sdk");
  return s_api_level;
}

static std::string current_abi() {
#if defined(__arm__)
  return "armeabi-v7a";
#elif defined(__aarch64__)
  return "arm64-v8a";
#elif defined(__i686__)
  return "x86";
#elif defined(__x86_64__)
  return "x86_64";
#else
  return "Unknown ABI";
#endif
}
}  // namespace FOREVER
