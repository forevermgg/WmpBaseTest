
#ifndef STACKTRACE_H_
#define STACKTRACE_H_

#include <string>
#include <vector>

namespace FOREVER {
namespace STACKTRACE {

// StackTrace acquires a stack trace for the current thread (not suitable for
// calling in an interrupt handler) on construction and supports conversion of
// raw stack pointers to a string of symbolic function names. Currently supports
// Android, OSX, iOS, Linux, and Windows -- on Windows addresses only, no
// symbols. Unsupported platforms will produce an empty stack.
//
class StackTrace {
 public:
  StackTrace();
  ~StackTrace() {}
  // Returns the stack as a vector of addresses.
  const std::vector<void*>& GetAddresses() const { return addresses_; }
  // Returns the stack as a vector of symbol names.
  const std::vector<std::string>& GetSymbols() const;
  // Returns formatted string containing symbolic function names for elements of
  // the stack trace.
  const std::string GetSymbolString() const;

 private:
  // Obtain symbol information for addresses.
  void ObtainSymbols() const;

  std::vector<void*> addresses_;
  // Mutable to support lazy evaluation.
  mutable std::vector<std::string> symbols_;
  mutable std::vector<uintptr_t> offsets_;
  mutable std::vector<std::string> modules_;
};

}  // namespace STACKTRACE
}  // namespace FOREVER

#endif  // STACKTRACE_H_
