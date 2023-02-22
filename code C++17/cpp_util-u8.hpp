#pragma once
#include "cpp_util-basic-types.hpp"         // Byte
#include "cpp_util-type-builders.hpp"       // in_, ref_, array_of_
#include "cpp_util-containers.hpp"          // is_empty

#include <string>
#include <string_view>      // std::string_view
#include <vector>

#include <assert.h>

namespace cpp_util::u8 {
    using   std::string,
            std::string_view,
            std::vector;
            
    constexpr int max_sequence_length = 4;

    constexpr auto is_continuation_byte( const Byte value )
        -> bool
    {
        constexpr auto  head_mask       = Byte( 0b1100'0000 );
        constexpr auto  head_pattern    = Byte( 0b1000'0000 );
        
        return ((value & head_mask) == head_pattern);
    }

    constexpr auto n_seq_bytes_of( const_<const char*> p_seq_start )
        -> int
    {
        const Byte lead_byte_bits = *p_seq_start;

        // For professional use replace the assertions with exception throwing.
        if( (lead_byte_bits & (1u << 7)) == 0 ) { return 1; }
        assert( (lead_byte_bits & (1u << 6)) != 0
            or !"Invalid lead byte (bit pattern reserved for continuation_bytes)." );
        if( (lead_byte_bits & (1u << 5)) == 0 ) { return 2; }
        if( (lead_byte_bits & (1u << 4)) == 0 ) { return 3; }
        if( (lead_byte_bits & (1u << 3)) == 0 ) { return 4; }
        assert( !"Invalid lead byte (specifies a more than 4 bytes sequence)." );
        for( ;; ) {}    // Should never get here.
    }

    constexpr auto n_code_points_in( in_<string_view> s )
        -> int
    {
        int result = 0;
        for( const Byte value: s ) {
            result += not is_continuation_byte( value );
        }
        return result;
    }
    
    template< class It >
    constexpr void advance( ref_<It> it )
    {
        it += n_seq_bytes_of( &*it );
    }

    template< class It >
    constexpr auto next( const_<It> it )
        -> It
    { It result = it; advance( result ); return result; }
    
    template< class It >
    constexpr void unadvance( ref_<It> it )
    {
        do{ --it; } while( is_continuation_byte( *it ) );
    }

    template< class It >
    constexpr auto prev( const_<It> it )
        -> It
    { It result = it; unadvance( result ); return result; }

    class Codepoint
    {
        static constexpr int buf_size = max_sequence_length;
        array_of_<buf_size, char>   m_units;    // With 8-bit bytes this is 32 bits.
        
    public:
        constexpr Codepoint(): m_units() {}

        constexpr Codepoint( const char ch ): m_units{ ch } {}

        constexpr explicit Codepoint( const_<const char*> p_seq_start )
            : m_units()
        {
            const int n = n_seq_bytes_of( p_seq_start );
            for( int i = 0; i < n; ++i ) { m_units[i] = p_seq_start[i]; }
        }
        
        constexpr auto data() const -> const char*  { return m_units; }
        constexpr auto size() const -> int          { return n_seq_bytes_of( m_units ); }
        constexpr auto sv() const -> string_view    { return {data(), 1u*size()}; }
    };

    class Composer
    {
        vector<Codepoint>   m_codepoints;
        
    public:
        Composer() {}

        explicit Composer( const int width ): m_codepoints( width, ' ' ) {}

        Composer( in_<string_view> s )
        {
            const_<const char*> p_end = s.data() + s.size();
           for( const char* p = s.data(); p < p_end; advance( p ) ) {
                m_codepoints.emplace_back( p );
            }
        }
        
        friend auto is_empty( in_<Composer> c ) -> bool { return is_empty( c.m_codepoints ); }

        auto size() const -> int { return int_size_of( m_codepoints ); }

        auto operator[]( const int i ) const -> string_view { return m_codepoints[i].sv(); }

        auto str() const
            -> string
        {
            string result;
            for( in_<Codepoint> cp: m_codepoints ) { result += cp.sv(); }
            return result;
        }

        void ensure_size( const int size )
        {
            if( int_size_of( m_codepoints ) < size ) { m_codepoints.resize( size, ' ' ); }
        }

        auto sized_for_index( const int i )
            -> Composer&
        {
            ensure_size( i + 1 );
            return *this;
        }

        void put_at( const int i, in_<string_view> s )
        {
            ensure_size( i + n_code_points_in( s ) );
            const_<const char*> p_end = s.data() + s.size();
            int j = i;
            for( const char* p = s.data(); p < p_end; advance( p ) ) {
                m_codepoints[j++] = Codepoint( p );
            }
        }
    };
}  // namespace cpp_util::u8
