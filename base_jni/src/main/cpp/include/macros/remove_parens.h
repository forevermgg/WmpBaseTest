
#ifndef FOREVER_MACROS_REMOVE_PARENS_H_
#define FOREVER_MACROS_REMOVE_PARENS_H_

#include "if.h"
#include "is_empty.h"

// A macro that removes at most one outer set of parentheses from its arguments.
// If the arguments are not surrounded by parentheses, this expands to the
// arguments unchanged. For example:
// `FOREVER_REMOVE_PARENS()` -> ``
// `FOREVER_REMOVE_PARENS(foo)` -> `foo`
// `FOREVER_REMOVE_PARENS(foo(1))` -> `foo(1)`
// `FOREVER_REMOVE_PARENS((foo))` -> `foo`
// `FOREVER_REMOVE_PARENS((foo(1)))` -> `foo(1)`
// `FOREVER_REMOVE_PARENS((foo)[1])` -> `(foo)[1]`
// `FOREVER_REMOVE_PARENS(((foo)))` -> `(foo)`
// `FOREVER_REMOVE_PARENS(foo, bar, baz)` -> `foo, bar, baz`
// `FOREVER_REMOVE_PARENS(foo, (bar), baz)` -> `foo, (bar), baz`
#define FOREVER_REMOVE_PARENS(...)                                            \
  FOREVER_IF(FOREVER_INTERNAL_IS_PARENTHESIZED(__VA_ARGS__), FOREVER_INTERNAL_ECHO, \
          FOREVER_INTERNAL_EMPTY())                                           \
  __VA_ARGS__

#define FOREVER_INTERNAL_IS_PARENTHESIZED(...) \
  FOREVER_IS_EMPTY(FOREVER_INTERNAL_EAT __VA_ARGS__)
#define FOREVER_INTERNAL_EAT(...)
#define FOREVER_INTERNAL_ECHO(...) __VA_ARGS__
#define FOREVER_INTERNAL_EMPTY()

#endif  // FOREVER_MACROS_REMOVE_PARENS_H_
