
#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <type_traits>

namespace FOREVER {
// like std::remove_cvref(_t)
#ifdef __cpp_lib_remove_cvref
using std::remove_cvref;
using std::remove_cvref_t;
#else
template <typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T> >;
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T> >;
#endif  // __cpp_lib_remove_cvref
}  // namespace FOREVER

namespace FOREVER {
// like std::type_identity(_t)
#ifdef __cpp_lib_type_identity
using std::type_identity;
using std::type_identity_t;
#else
template <typename T>
struct type_identity {
  using type = T;
};
template <typename T>
using type_identity_t = typename type_identity<T>::type;
#endif  // __cpp_lib_type_identity
}  // namespace FOREVER

#endif /* TYPE_TRAITS_H */
