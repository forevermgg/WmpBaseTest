
#ifndef UTILS_ARCHITECTURE_H
#define UTILS_ARCHITECTURE_H

#include <stddef.h>

namespace FOREVER {

constexpr size_t CACHELINE_SIZE = 64;

namespace ARCH {

size_t getPageSize() noexcept;

} // namespace ARCH
} // namespace FOREVER

#endif // UTILS_ARCHITECTURE_H
