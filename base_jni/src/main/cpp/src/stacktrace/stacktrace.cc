
#include "stacktrace.h"

#include <assert.h>  // For checking return values (port has no logging).
#include <cxxabi.h>
#include <dlfcn.h>
#include <string.h>
#include <unwind.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace FOREVER {
namespace STACKTRACE {
struct BacktraceFrame {
  void** current;
  void** end;
};

// Callback used by Unwind to obtain the program counter address.
_Unwind_Reason_Code UnwindCallback(_Unwind_Context* context, void* arg) {
  BacktraceFrame* frame = reinterpret_cast<BacktraceFrame*>(arg);
  uintptr_t pc = _Unwind_GetIP(context);
  if (pc) {
    if (frame->current == frame->end) {
      return _URC_END_OF_STACK;
    } else {
      *frame->current++ = reinterpret_cast<void*>(pc);
    }
  }
  return _URC_NO_REASON;
}

// Writes the addresses of the current stack into |addresses| up to |size| deep.
// Returns the number of actual frames written.
int GetBacktrace(void** addresses, int size) {
  BacktraceFrame frame;
  frame.current = addresses;
  frame.end = addresses + size;

  _Unwind_Backtrace(UnwindCallback, &frame);
  int count = static_cast<int>(frame.current - addresses);
  return count;
}

StackTrace::StackTrace() {
  const int kMaxStackTraceDepth = 128;  // Arbitrary limit.
  addresses_.resize(kMaxStackTraceDepth);
  const int depth = GetBacktrace(&addresses_[0], kMaxStackTraceDepth);
  assert(depth > 0);
  addresses_.resize(depth);
}

void StackTrace::ObtainSymbols() const {
  symbols_.resize(addresses_.size());
  offsets_.resize(addresses_.size());
  modules_.resize(addresses_.size());
  for (size_t i = 0; i < addresses_.size(); ++i) {
    const char* symbol = "";

    Dl_info info;
    if (dladdr(addresses_[i], &info)) {
      if (info.dli_sname) symbol = info.dli_sname;
      if (info.dli_fname) modules_[i] = std::string(info.dli_fname);
      offsets_[i] = reinterpret_cast<uintptr_t>(addresses_[i]) -
                    reinterpret_cast<uintptr_t>(info.dli_saddr);
    }

    int status = 0;
    char* demangled =
        __cxxabiv1::__cxa_demangle(symbol, nullptr, nullptr, &status);
    if (!demangled) demangled = strdup(symbol);

    symbols_[i] = demangled;
    free(demangled);
  }
}

const std::vector<std::string>& StackTrace::GetSymbols() const {
  if (symbols_.empty()) ObtainSymbols();
  return symbols_;
}

const std::string StackTrace::GetSymbolString() const {
  const std::vector<void*>& addresses = GetAddresses();
  const std::vector<std::string>& symbols = GetSymbols();
  assert(addresses.size() == symbols.size());
  if (!modules_.empty()) assert(modules_.size() == addresses.size());
  if (!offsets_.empty()) assert(offsets_.size() == addresses.size());
  std::ostringstream output;
  // The first two frames are StackTrace itself.
  // NOTE: If we later add an alternative Windows implementation based on
  // RtlVirtualUnwind, the start index will need to become platform-specific,
  // since RtlVirtualUnwind does not return for the caller's frame and thus
  // GetBacktrace() is not present in the trace.
  for (size_t i = 2; i < addresses.size(); ++i) {
    // Output format is similar to the Android debuggerd format:
    // #XX pc <address> <module> (<proc>+<offset>)

    // Frame counter.
    output << "#" << std::setfill('0') << std::setw(2) << std::dec << (i - 2)
           << " ";
    // Address.
    output << "pc " << std::setfill('0') << std::setw(16) << std::hex
           << reinterpret_cast<uintptr_t>(addresses[i]) << " ";
    // Module name.
    if (!modules_.empty()) output << modules_[i] << " ";
    // Symbol (mangled or demangled).
    if (!symbols[i].empty()) {
      output << "(" << symbols[i];
      // Instruction byte offset.
      if (!offsets_.empty()) output << "+" << std::dec << offsets_[i] << ")";
    }
    output << std::endl;
  }
  return output.str();
}
}  // namespace STACKTRACE
}  // namespace FOREVER
