#pragma once
#include <limits.h>         // CHAR_BIT
#include <stddef.h>         // size_t

namespace cpp_util {
    using Byte = unsigned char;
    template< class T > constexpr int bits_per_ = sizeof( T )*CHAR_BIT;
}  // namespace cpp_util
