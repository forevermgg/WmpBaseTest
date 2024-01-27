
#ifndef PUBLIC_COMPILER_H_
#define PUBLIC_COMPILER_H_

#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
#define FOREVER_LIKELY(_x) __builtin_expect(!!(_x), 1)
#define FOREVER_UNLIKELY(_x) __builtin_expect(!!(_x), 0)
#else
#define FOREVER_LIKELY(_x) (_x)
#define FOREVER_UNLIKELY(_x) (_x)
#endif

// FOREVER_STATIC_CAST(TYPE, VAL): avoids the -Wold-style-cast warning when
// writing code that needs to be compiled as C and C++.
#ifdef __cplusplus
#define FOREVER_STATIC_CAST(TYPE, VAL) static_cast<TYPE>(VAL)
#else
#define FOREVER_STATIC_CAST(TYPE, VAL) ((TYPE)(VAL))
#endif

// FOREVER_REINTERPRET_CAST(TYPE, VAL): avoids the -Wold-style-cast warning
// when writing code that needs to be compiled as C and C++.
#ifdef __cplusplus
#define FOREVER_REINTERPRET_CAST(TYPE, VAL) reinterpret_cast<TYPE>(VAL)
#else
#define FOREVER_REINTERPRET_CAST(TYPE, VAL) ((TYPE)(VAL))
#endif

// FOREVER_NULL: avoids the -Wzero-as-null-pointer-constant warning when
// writing code that needs to be compiled as C and C++.
#ifdef __cplusplus
#define FOREVER_NULL nullptr
#else
#define FOREVER_NULL NULL
#endif

#endif  // PUBLIC_COMPILER_H_
