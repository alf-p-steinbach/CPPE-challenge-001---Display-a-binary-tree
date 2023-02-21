#pragma once
#include "cpp_util-basic-types.hpp"         // bits_per_

#include <random>

#include <assert.h>         // assert
#include <stdint.h>         // uint32_t, uint64_t

namespace cpp_util::random {
    enum Seed: uint64_t {};
    
    inline auto entropy32()
        -> uint32_t
    {
        static std::random_device rd;
        return rd();
    }

    inline auto entropy64()
        -> uint64_t
    { return (uint64_t( entropy32() ) << 32) | entropy32(); }

    class Integers
    {
        std::mt19937_64                     m_bits_generator;
        std::uniform_int_distribution<>     m_integers_generator;

    public:
        Integers( const int n_distinct, const Seed seed = Seed( entropy64() ) ):
            m_bits_generator( seed ),
            m_integers_generator( 0, n_distinct - 1 )
        {
            assert( n_distinct > 1 );
        }
        
        Integers( const int lowest, const int highest, const Seed seed = Seed( entropy64() ) ):
            m_bits_generator( seed ),
            m_integers_generator( lowest, highest )
        {
            assert( lowest < highest );
        }

        auto operator()() -> int { return m_integers_generator( m_bits_generator ); }
    };
    
}  // namespace cpp_util::random
