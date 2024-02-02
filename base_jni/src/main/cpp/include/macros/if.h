
#ifndef FOREVER_MACROS_IF_H_
#define FOREVER_MACROS_IF_H_

#include "concat.h"

// Given a `_Cond` that evaluates to exactly 0 or 1, this macro evaluates to
// either the `_Then` or `_Else` args. Unlike a real conditional expression,
// this does not support conditions other than `0` and `1`.
#define FOREVER_IF(_Cond, _Then, _Else) \
  FOREVER_CONCAT(FOREVER_INTERNAL_IF_, _Cond)(_Then, _Else)

// Implementation details: do not use directly.
#define FOREVER_INTERNAL_IF_1(_Then, _Else) _Then
#define FOREVER_INTERNAL_IF_0(_Then, _Else) _Else

#endif  // FOREVER_MACROS_IF_H_
