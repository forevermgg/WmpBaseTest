
#ifndef FOREVER_MACROS_UNIQUIFY_H_
#define FOREVER_MACROS_UNIQUIFY_H_

#include "concat.h"

// A macro to create a "unique" token name beginning with `name` by appending
// the current line number. This is generally used inside other macros that need
// to create temporaries while minimizing the likelihood of name conflicts.
#define FOREVER_UNIQUIFY(name) FOREVER_CONCAT(name, __LINE__)

#endif  // FOREVER_MACROS_UNIQUIFY_H_
