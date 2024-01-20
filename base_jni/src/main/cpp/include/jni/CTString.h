#include <array>
#include <cstddef>
#include <iostream>

namespace QtJniTypes {

// a constexpr type for string literals of any character width, aware of the
// length of the string.
template <size_t N_WITH_NULL, typename BaseType = char>
struct CTString {
  BaseType m_data[N_WITH_NULL] = {};

  constexpr CTString() noexcept {}

  // Can be instantiated (only) with a string literal
  constexpr explicit CTString(const BaseType (&data)[N_WITH_NULL]) noexcept {
    for (size_t i = 0; i < N_WITH_NULL - 1; ++i) m_data[i] = data[i];
  }

  constexpr BaseType at(size_t i) const { return m_data[i]; }

  constexpr BaseType operator[](size_t i) const { return at(i); }

  static constexpr size_t size() noexcept { return N_WITH_NULL; }

  constexpr operator const BaseType *() const noexcept { return m_data; }

  constexpr const BaseType *data() const noexcept { return m_data; }

  template <size_t N2_WITH_NULL>
  constexpr bool startsWith(
      const BaseType (&lit)[N2_WITH_NULL]) const noexcept {
    if constexpr (N2_WITH_NULL > N_WITH_NULL) {
      return false;
    } else {
      for (size_t i = 0; i < N2_WITH_NULL - 1; ++i) {
        if (m_data[i] != lit[i]) return false;
      }
    }
    return true;
  }

  constexpr bool startsWith(BaseType c) const noexcept {
    return N_WITH_NULL > 1 && m_data[0] == c;
  }

  template <size_t N2_WITH_NULL>
  constexpr bool endsWith(const BaseType (&lit)[N2_WITH_NULL]) const noexcept {
    if constexpr (N2_WITH_NULL > N_WITH_NULL) {
      return false;
    } else {
      for (size_t i = 0; i < N2_WITH_NULL; ++i) {
        if (m_data[N_WITH_NULL - i - 1] != lit[N2_WITH_NULL - i - 1])
          return false;
      }
    }
    return true;
  }

  constexpr bool endsWith(BaseType c) const noexcept {
    return N_WITH_NULL > 1 && m_data[N_WITH_NULL - 2] == c;
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr bool operator==(
      const CTString<N_WITH_NULL> &lhs,
      const CTString<N2_WITH_NULL> &rhs) noexcept {
    if constexpr (N_WITH_NULL != N2_WITH_NULL) {
      return false;
    } else {
      for (size_t i = 0; i < N_WITH_NULL - 1; ++i) {
        if (lhs.at(i) != rhs.at(i)) return false;
      }
    }
    return true;
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr bool operator!=(
      const CTString<N_WITH_NULL> &lhs,
      const CTString<N2_WITH_NULL> &rhs) noexcept {
    return !operator==(lhs, rhs);
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr bool operator==(
      const CTString<N_WITH_NULL> &lhs,
      const BaseType (&rhs)[N2_WITH_NULL]) noexcept {
    return operator==(lhs, CTString<N2_WITH_NULL>(rhs));
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr bool operator==(
      const BaseType (&lhs)[N2_WITH_NULL],
      const CTString<N_WITH_NULL> &rhs) noexcept {
    return operator==(CTString<N2_WITH_NULL>(lhs), rhs);
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr bool operator!=(
      const CTString<N_WITH_NULL> &lhs,
      const BaseType (&rhs)[N2_WITH_NULL]) noexcept {
    return operator!=(lhs, CTString<N2_WITH_NULL>(rhs));
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr bool operator!=(
      const BaseType (&lhs)[N2_WITH_NULL],
      const CTString<N_WITH_NULL> &rhs) noexcept {
    return operator!=(CTString<N2_WITH_NULL>(lhs), rhs);
  }

  template <size_t N2_WITH_NULL>
  friend inline constexpr auto operator+(
      const CTString<N_WITH_NULL> &lhs,
      const CTString<N2_WITH_NULL> &rhs) noexcept {
    char data[N_WITH_NULL + N2_WITH_NULL - 1] = {};
    for (size_t i = 0; i < N_WITH_NULL - 1; ++i) data[i] = lhs[i];
    for (size_t i = 0; i < N2_WITH_NULL - 1; ++i)
      data[N_WITH_NULL - 1 + i] = rhs[i];
    return CTString<N_WITH_NULL + N2_WITH_NULL - 1>(data);
  }
};
}  // namespace QtJniTypes