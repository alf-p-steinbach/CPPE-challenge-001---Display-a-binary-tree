#pragma once
#include <iterator>         // std::size

#include "cpp_util-type-builders.hpp"       // in_

namespace cpp_util {
    template< class Container >
    inline auto is_empty( in_<Container> c ) -> bool { return c.empty(); }
    
    template< class Container >
    inline auto int_size_of( in_<Container> c ) -> int { return static_cast<int>( std::size( c ) ); }
}  // namespace cpp_util
