#ifndef FOREVER_STRING_PIECE_H_
#define FOREVER_STRING_PIECE_H_

#include <string_view>

namespace FOREVER {

using StringPiece = std::string_view;
using StringPiece16 = std::u16string_view;

}  // namespace base

#endif  // FOREVER_STRING_PIECE_H_
