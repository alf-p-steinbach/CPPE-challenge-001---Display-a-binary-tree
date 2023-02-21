#pragma once

#include <assert.h>         // assert
#include <type_traits>      // std::is_floating_point_v

namespace cpp_util {
    namespace impl
    {
        // Essentially this is Horner's rule adapted to calculating a power, so that the
        // number of floating point multiplications is at worst O(log₂n).
        template< class Number_type >
        constexpr inline auto intpow_( const Number_type base, const int exponent )
            -> Number_type
        {
            Number_type result = 1;
            Number_type weight = base;
            for( int n = exponent; n != 0; weight *= weight ) {
                if( n % 2 != 0 ) {
                    result *= weight;
                }
                n /= 2;
            }
            return result;
        }
    }  // namespace impl

    template< class Number_type >
    constexpr inline auto intpow( const Number_type base, const int exponent )
        -> Number_type
    {
        // TODO: proper failure handling
        if( exponent < 0 and not std::is_floating_point_v<Number_type> ) {
            assert( !"For negative powers the base must be floating point" );
        }
        return (false?0
            : exponent > 0?     impl::intpow_<Number_type>( base, exponent )
            : exponent == 0?    1
            :                   Number_type( 1.0/impl::intpow_<Number_type>( base, -exponent ) )
            );
    }
}  // namespace cpp_util
