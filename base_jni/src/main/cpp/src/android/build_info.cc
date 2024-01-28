
#include "android/build_info.h"

#include <sys/system_properties.h>

#include <string>

namespace FOREVER {
BuildInfo::BuildInfo() {}
// static
BuildInfo* BuildInfo::GetInstance() {
  return Singleton<BuildInfo>::GetInstance();
}

std::string BuildInfo::HardwareModelName() {
  char device_model_str[PROP_VALUE_MAX];
  __system_property_get("ro.product.model", device_model_str);
  return std::string(device_model_str);
}

std::string BuildInfo::OperatingSystemName() { return "Android"; }

std::string BuildInfo::GetAndroidBuildCodename() {
  char os_version_codename_str[PROP_VALUE_MAX];
  __system_property_get("ro.build.version.codename", os_version_codename_str);
  return std::string(os_version_codename_str);
}

std::string BuildInfo::GetAndroidBuildID() {
  char os_build_id_str[PROP_VALUE_MAX];
  __system_property_get("ro.build.id", os_build_id_str);
  return std::string(os_build_id_str);
}

std::string BuildInfo::GetAndroidHardwareEGL() {
  char os_hardware_egl_str[PROP_VALUE_MAX];
  __system_property_get("ro.hardware.egl", os_hardware_egl_str);
  return std::string(os_hardware_egl_str);
}
}  // namespace FOREVER
