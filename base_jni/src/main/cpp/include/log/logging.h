#ifndef FOREVER_LOGGING_H_
#define FOREVER_LOGGING_H_

#include <sstream>

#include "log_level.h"
#include "macros/macros.h"

namespace FOREVER {

namespace testing {
struct LogCapture {
  LogCapture();
  ~LogCapture();

  std::string str() const;

 private:
  std::ostringstream stream_;
};
}  // namespace testing

class LogMessageVoidify {
 public:
  void operator&(std::ostream&) {}
};

class LogMessage {
 public:
  LogMessage(LogSeverity severity,
             const char* file,
             int line,
             const char* condition);
  ~LogMessage();

  std::ostream& stream() { return stream_; }

  static void CaptureNextLog(std::ostringstream* stream);

 private:
  // This is a raw pointer so that we avoid having a non-trivially-destructible
  // static. It is only ever for use in unit tests.
  static thread_local std::ostringstream* capture_next_log_stream_;
  std::ostringstream stream_;
  const LogSeverity severity_;
  const char* file_;
  const int line_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

// Gets the FOREVER_VLOG default verbosity level.
int GetVlogVerbosity();

// Returns true if |severity| is at or above the current minimum log level.
// kLogFatal and above is always true.
bool ShouldCreateLogMessage(LogSeverity severity);

[[noreturn]] void KillProcess();

}  // namespace FOREVER

#define FOREVER_LOG_STREAM(severity) \
  ::FOREVER::LogMessage(::FOREVER::LOG_##severity, __FILE__, __LINE__, nullptr).stream()

#define FOREVER_LAZY_STREAM(stream, condition) \
  !(condition) ? (void)0 : ::FOREVER::LogMessageVoidify() & (stream)

#define FOREVER_EAT_STREAM_PARAMETERS(ignored) \
  true || (ignored)                        \
      ? (void)0                            \
      : ::FOREVER::LogMessageVoidify() &       \
            ::FOREVER::LogMessage(::FOREVER::kLogFatal, 0, 0, nullptr).stream()

#define FOREVER_LOG_IS_ON(severity) \
  (::FOREVER::ShouldCreateLogMessage(::FOREVER::LOG_##severity))

#define FOREVER_LOG(severity) \
  FOREVER_LAZY_STREAM(FOREVER_LOG_STREAM(severity), FOREVER_LOG_IS_ON(severity))

#define FOREVER_CHECK(condition)                                              \
  FOREVER_LAZY_STREAM(                                                        \
      ::FOREVER::LogMessage(::FOREVER::kLogFatal, __FILE__, __LINE__, #condition) \
          .stream(),                                                      \
      !(condition))

#define FOREVER_VLOG_IS_ON(verbose_level) \
  ((verbose_level) <= ::FOREVER::GetVlogVerbosity())

// The VLOG macros log with negative verbosities.
#define FOREVER_VLOG_STREAM(verbose_level) \
  ::FOREVER::LogMessage(-verbose_level, __FILE__, __LINE__, nullptr).stream()

#define FOREVER_VLOG(verbose_level) \
  FOREVER_LAZY_STREAM(FOREVER_VLOG_STREAM(verbose_level), FOREVER_VLOG_IS_ON(verbose_level))

#ifndef NDEBUG
#define FOREVER_DLOG(severity) FOREVER_LOG(severity)
#define FOREVER_DCHECK(condition) FOREVER_CHECK(condition)
#else
#define FOREVER_DLOG(severity) FOREVER_EAT_STREAM_PARAMETERS(true)
#define FOREVER_DCHECK(condition) FOREVER_EAT_STREAM_PARAMETERS(condition)
#endif

#define FOREVER_UNREACHABLE()                          \
  {                                                \
    FOREVER_LOG(ERROR) << "Reached unreachable code."; \
    ::FOREVER::KillProcess();                          \
  }

#endif  // FOREVER_LOGGING_H_
