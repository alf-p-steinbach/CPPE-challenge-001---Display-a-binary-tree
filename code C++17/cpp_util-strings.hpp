#pragma once
#include <algorithm>        // std::max
#include <string>           // std::string
#include <string_view>      // std::string_view

#include "cpp_util-type-builders.hpp"       // in_

namespace cpp_util {
    inline auto n_digits_of( const int v )
        -> int
    {
        int result = 0;
        for( int digits = v; digits != 0; digits /= 10 ) { ++result; }
        return result;
    }

    inline auto n_chars_of( const int v ) -> int { return n_digits_of( v ) + (v < 0); }

    template< class It >
    inline auto join_strings( const It first, const It beyond )
        -> std::string
    {
        std::string result;
        for( It it = first; it != beyond; ++it ) { result += *it; }
        return result;
    }

    inline auto spaces( const int n )
        -> std::string
    { return std::string( std::max( n, 0 ), ' ' ); }

    inline auto at_right_in( const int w, in_<std::string_view> s )
        -> std::string
    { return spaces( w - int_size_of( s ) ) + std::string( s ); }
}  // namespace cpp_util
