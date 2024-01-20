#include "log/log_settings.h"

#include <fcntl.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include "log/log_level.h"
#include "log/logging.h"

namespace FOREVER {
namespace STATE {

// Defined in log_settings_state.cc.
extern LogSettings g_log_settings;

}  // namespace state

void SetLogSettings(const LogSettings& settings) {
  // Validate the new settings as we set them.
  STATE::g_log_settings.min_log_level =
      std::min(kLogFatal, settings.min_log_level);
}

LogSettings GetLogSettings() {
  return STATE::g_log_settings;
}

int GetMinLogLevel() {
  return std::min(STATE::g_log_settings.min_log_level, kLogFatal);
}

ScopedSetLogSettings::ScopedSetLogSettings(const LogSettings& settings) {
  old_settings_ = GetLogSettings();
  SetLogSettings(settings);
}

ScopedSetLogSettings::~ScopedSetLogSettings() {
  SetLogSettings(old_settings_);
}

}  // namespace FOREVER
