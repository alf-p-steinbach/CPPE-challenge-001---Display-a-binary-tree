#pragma once

namespace cpp_util {
    template< class Type > using const_ = const Type;       // Generally useful.
    template< class Type > using ref_   = Type&;            // Generally useful.
    template< class Type > using in_    = const Type&;      // Mostly just for in-parameters.
    
    template< int n, class Item > using array_of_ = Item[n];
}  // namespace cpp_util
