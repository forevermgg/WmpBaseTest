
#include "android/android_utils.h"
#include <string>
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/stat.h>  /* for fdstat() */
#include <fcntl.h>
#include <sys/system_properties.h>
#include <mutex>


namespace FOREVER {
uint32_t sApiLevel = 0;
std::once_flag sApiLevelOnceFlag;

static int api_level() {
  std::call_once(sApiLevelOnceFlag, []() {
      char sdkVersion[PROP_VALUE_MAX];
      __system_property_get("ro.build.version.sdk", sdkVersion);
            sApiLevel = atoi(sdkVersion);
    }
  );
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
void __system_property_read_callback(
     const prop_info* info,
     void (*callback)(void* cookie, const char* name, const char* value, uint32_t serial),
     void* cookie) __attribute__((weak));

/* Callback used with __system_property_read_callback. */
static void prop_read_int(void* cookie, const char* name, const char* value, uint32_t serial) {
  *(int *)cookie = atoi(value);
  (void)name;
  (void)serial;
}

static int system_property_get_int(const char* name) {
  int result = 0;
  if (__system_property_read_callback) {
    const prop_info* info = __system_property_find(name);
    if (info)
      __system_property_read_callback(info, &prop_read_int, &result);
  } else {
    char value[PROP_VALUE_MAX] = {};
    if (__system_property_get(name, value) >= 1)
      result = atoi(value);
  }
  return result;
}

static int device_api_level() {
  static int s_api_level = -1;
  if (s_api_level < 0)
    s_api_level = system_property_get_int("ro.build.version.sdk");
  return s_api_level;
}

}  // namespace FOREVER
