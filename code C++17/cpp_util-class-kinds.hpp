#pragma once

#ifdef _MSC_VER
#   pragma warning( disable: 4624 )     // "destructor was implicitly defined as deleted"
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
    
    class Non_instantiable
    {
    public:
        ~Non_instantiable() = delete;
        auto operator new( size_t ) -> void* = delete;      // Not hack-proof, but.
        auto operator new[]( size_t ) -> void* = delete;
    };
}  // namespace cpp_util
