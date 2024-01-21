
#ifndef FOREVER_CONTEXT_H_
#define FOREVER_CONTEXT_H_

#include <stdint.h>

#include <functional>
#include <memory>

#include "connectivity_manager.h"

namespace FOREVER {

// Context defines platform implementation-related interfaces. Nearby Sharing
// components should access these interfaces under their environment.
// On different platforms, the implementation of these interfaces are different.
// In order to support test cases on Google3, Nearby Sharing SDK also provides
// a mock implementation of these interfaces.
class Context {
 public:
  virtual ~Context() = default;

  virtual ConnectivityManager* GetConnectivityManager() const = 0;
};

}  // namespace FOREVER

#endif  // FOREVER_CONTEXT_H_
