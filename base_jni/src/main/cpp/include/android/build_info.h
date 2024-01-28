
#ifndef ANDROID_BUILD_INFO_H_
#define ANDROID_BUILD_INFO_H_

#include <jni.h>

#include <string>
#include <vector>

#include "../memory/singleton_stdmutex.h"

namespace FOREVER {
class BuildInfo : public MEMORY::Singleton<BuildInfo> {
 public:
  BuildInfo(const BuildInfo&) = delete;
  BuildInfo& operator=(const BuildInfo&) = delete;
  ~BuildInfo() {}
  static BuildInfo* GetInstance();
  explicit BuildInfo();

  std::string HardwareModelName();
  std::string OperatingSystemName();
  std::string GetAndroidBuildCodename();
  std::string GetAndroidBuildID();
  std::string GetAndroidHardwareEGL();
};

}  // namespace FOREVER

#endif  // ANDROID_BUILD_INFO_H_
