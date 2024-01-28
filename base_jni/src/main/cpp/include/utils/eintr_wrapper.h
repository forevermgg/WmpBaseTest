
#ifndef EINTR_WRAPPER_H_
#define EINTR_WRAPPER_H_

#include <errno.h>

#include "build_config.h"

#if defined(FOREVER_OS_WIN)

// Windows has no concept of EINTR.
#define FOREVER_HANDLE_EINTR(x) (x)
#define FOREVER_IGNORE_EINTR(x) (x)

#else

#define FOREVER_HANDLE_EINTR(x)                                 \
  ({                                                        \
    decltype(x) eintr_wrapper_result;                       \
    do {                                                    \
      eintr_wrapper_result = (x);                           \
    } while (eintr_wrapper_result == -1 && errno == EINTR); \
    eintr_wrapper_result;                                   \
  })

#define FOREVER_IGNORE_EINTR(x)                               \
  ({                                                      \
    decltype(x) eintr_wrapper_result;                     \
    do {                                                  \
      eintr_wrapper_result = (x);                         \
      if (eintr_wrapper_result == -1 && errno == EINTR) { \
        eintr_wrapper_result = 0;                         \
      }                                                   \
    } while (0);                                          \
    eintr_wrapper_result;                                 \
  })

#endif  // defined(FOREVER_OS_WIN)

#endif  // EINTR_WRAPPER_H_
