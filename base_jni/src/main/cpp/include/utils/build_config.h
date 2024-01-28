
// This file adds defines about the platform we're currently building on.
//  Operating System:
//    FOREVER_OS_WIN / FOREVER_OS_MACOSX / FOREVER_OS_LINUX / FOREVER_OS_POSIX (MACOSX or LINUX)
//    / FOREVER_OS_NACL (NACL_SFI or NACL_NONSFI) / FOREVER_OS_NACL_SFI /
//    FOREVER_OS_NACL_NONSFI
//  Compiler:
//    COMPILER_MSVC / COMPILER_GCC
//  Processor:
//    FOREVER_ARCH_CPU_X86 / FOREVER_ARCH_CPU_X86_64 / FOREVER_ARCH_CPU_X86_FAMILY (X86 or
//    X86_64) FOREVER_ARCH_CPU_32_BITS / FOREVER_ARCH_CPU_64_BITS

#ifndef BUILD_CONFIG_H_
#define BUILD_CONFIG_H_

#if defined(__Fuchsia__)
#define OS_FUCHSIA 1
#elif defined(ANDROID)
#define FOREVER_OS_ANDROID 1
#elif defined(__APPLE__)
// only include TargetConditions after testing ANDROID as some android builds
// on mac don't have this header available and it's not needed unless the target
// is really mac/ios.
#include <TargetConditionals.h>
#define FOREVER_OS_MACOSX 1
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define FOREVER_OS_IOS 1
#endif  // defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#if defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR
#define FOREVER_OS_IOS_SIMULATOR 1
#endif  // defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#elif defined(__linux__)
#define FOREVER_OS_LINUX 1
// include a system header to pull in features.h for glibc/uclibc macros.
#include <unistd.h>
#if defined(__GLIBC__) && !defined(__UCLIBC__)
// we really are using glibc, not uClibc pretending to be glibc
#define LIBC_GLIBC 1
#endif
#elif defined(_WIN32)
#define FOREVER_OS_WIN 1
#elif defined(__FreeBSD__)
#define FOREVER_OS_FREEBSD 1
#elif defined(__OpenBSD__)
#define FOREVER_OS_OPENBSD 1
#elif defined(__sun)
#define FOREVER_OS_SOLARIS 1
#elif defined(__QNXNTO__)
#define FOREVER_OS_QNX 1
#else
#error Please add support for your platform in build_config.h
#endif

// For access to standard BSD features, use FOREVER_OS_BSD instead of a
// more specific macro.
#if defined(FOREVER_OS_FREEBSD) || defined(FOREVER_OS_OPENBSD)
#define FOREVER_OS_BSD 1
#endif

// For access to standard POSIXish features, use FOREVER_OS_POSIX instead of a
// more specific macro.
#if defined(FOREVER_OS_MACOSX) || defined(FOREVER_OS_LINUX) ||    \
    defined(FOREVER_OS_FREEBSD) || defined(FOREVER_OS_OPENBSD) || \
    defined(FOREVER_OS_SOLARIS) || defined(FOREVER_OS_ANDROID) || \
    defined(FOREVER_OS_NACL) || defined(FOREVER_OS_QNX)
#define FOREVER_OS_POSIX 1
#endif

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define FOREVER_ARCH_CPU_X86_FAMILY 1
#define FOREVER_ARCH_CPU_X86_64 1
#define FOREVER_ARCH_CPU_64_BITS 1
#define FOREVER_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#define FOREVER_ARCH_CPU_X86_FAMILY 1
#define FOREVER_ARCH_CPU_X86 1
#define FOREVER_ARCH_CPU_32_BITS 1
#define FOREVER_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#define FOREVER_ARCH_CPU_ARM_FAMILY 1
#define FOREVER_ARCH_CPU_ARMEL 1
#define FOREVER_ARCH_CPU_32_BITS 1
#define FOREVER_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__aarch64__)
#define FOREVER_ARCH_CPU_ARM_FAMILY 1
#define FOREVER_ARCH_CPU_ARM64 1
#define FOREVER_ARCH_CPU_64_BITS 1
#define FOREVER_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__pnacl__)
#define FOREVER_ARCH_CPU_32_BITS 1
#define FOREVER_ARCH_CPU_LITTLE_ENDIAN 1
#else
#error Please add support for your architecture in build_config.h
#endif

#endif  // BUILD_CONFIG_H_
