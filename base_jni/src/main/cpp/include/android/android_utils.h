
#ifndef ANDROID_UTILS_H_
#define ANDROID_UTILS_H_

#include <string>

namespace FOREVER {

static int api_level();
// Returns the value of the Android system property named `name`. If the
// property does not exist, returns an empty string (a non-existing property is
// the same as a property with an empty value for this API).
static std::string GetAndroidProp(const char* name);


static int  system_property_get_int(const char* name);

static int device_api_level();

static std::string current_abi();

static constexpr const char *kManufacturerSystemPropertyName = "ro.product.manufacturer";
static constexpr const char *kModelSystemPropertyName        = "ro.product.model";
static constexpr const char *kSDKSystemPropertyName          = "ro.build.version.sdk";
}  // namespace FOREVER

#endif  // ANDROID_UTILS_H_
