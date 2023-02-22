#pragma once

#ifdef _MSC_VER
#   pragma warning( disable: 4624 )         // "destructor was implicitly defined as deleted"
#endif

#include "cpp_util-type-builders.hpp"       // in_

namespace cpp_util {
    // A `Non_copyable` is neither copyable nor movable.
    class Non_copyable
    {
        using Self = Non_copyable;

        Non_copyable( in_<Self> ) = delete;
        auto operator=( in_<Self> ) -> Self& = delete;

        // For exposition only, it's the default:
        Non_copyable( Self&& ) = delete;
        auto operator=( Self&& ) -> Self& = delete;
        
    public:
        Non_copyable() {}
    };
    
    class Static_class  // non-instantiable
    {
    public:
        Static_class() = delete;
        Static_class( in_<Static_class> ) = delete;
    };
}  // namespace cpp_util
