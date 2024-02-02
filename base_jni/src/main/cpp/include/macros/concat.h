
#ifndef FOREVER_MACROS_CONCAT_H_
#define FOREVER_MACROS_CONCAT_H_

// A macro that expands to the concatenation of its arguments. If the arguments
// are themselves macros, they are first expanded (due to the indirection
// through a second macro). This can be used to construct tokens.
#define FOREVER_CONCAT(a, b) FOREVER_INTERNAL_CONCAT(a, b)

// Implementation details: do not use directly.
#define FOREVER_INTERNAL_CONCAT(a, b) a##b

#endif  // FOREVER_MACROS_CONCAT_H_
