#include "app-Binary_tree.hpp"

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <limits.h>

namespace app {
    namespace cppu      = cpp_util;
    namespace u8        = cpp_util::u8;
    namespace random    = cpp_util::random;

    using   cppu::const_, cppu::in_, cppu::is_empty, cppu::int_size_of,
            cppu::n_digits_of, cppu::intpow, cppu::n_chars_of,
            cppu::join_strings, cppu::at_right_in,
            cppu::contains;
    using   std::max,                           // <algorithm>
            std::function,                      // <functional>
            std::endl,                          // <iostream>
            std::set,                           // <set>
            std::string, std::to_string,        // <string>
            std::string_view,                   // <string_view>
            std::vector;                        // <vector>
            
    auto&   out = std::cout;

    class Displayer
    {
        const string        m_prefix;
        const int           m_indent;
        const int           m_value_width;
        const int           m_level_width;
        const Binary_tree&  m_tree;

        auto value_pos_of( const int level ) const
            -> int
        { return m_indent + level*m_level_width; }

    public:
        Displayer( in_<Binary_tree> tree, string prefix = "" ):
            m_prefix            ( move( prefix ) ),
            m_indent            ( int_size_of( m_prefix ) ),
            m_value_width       ( (is_empty( tree )
                ? 0
                : max( n_chars_of( tree.max_value() ), n_chars_of( tree.min_value() ) )
                ) ),
            m_level_width       ( m_value_width + 4 ),
            m_tree              ( tree )
        {}

        void display() const
        {
            using Vertical_connection_set = set<int>;   // Each value is level of the root of a connection line.

            struct Row
            {
                u8::Composer                line;
                Vertical_connection_set     vertical_connections;
                int                         node_level = INT_MAX;
            };
            
            Row previous;

            for( const auto [node_value, node_level]: m_tree ) {
                Row current = { u8::Composer( m_prefix ), {}, node_level };
     
                if( node_level > previous.node_level ) {
                    const bool two_child_nodes = contains( previous.node_level, previous.vertical_connections );
                    const int   i   = value_pos_of( previous.node_level ) + m_value_width;
                    const auto  s   = string() + "╺" + (two_child_nodes? "┥" : "┑");            // "┫" : "┓");
                    previous.line.put_at( i, s );
                    previous.vertical_connections.insert( previous.node_level );
                } else {
                    previous.vertical_connections.erase( previous.node_level );
                }

                current.vertical_connections = previous.vertical_connections;
                for( const int level: current.vertical_connections ) {
                    if( level == node_level ) {
                        current.line.put_at( value_pos_of( level ) + m_value_width, "╺" "┙" );  // "╺" "┛" );
                    } else {
                        current.line.put_at( value_pos_of( level ) + m_value_width + 1, "│" );  // "┃" );
                    }
                }

                const int value_pos = value_pos_of( node_level );
                current.line.put_at( value_pos, at_right_in( m_value_width, to_string( node_value ) ) );

                if( node_level > 0 ) {
                    const int i = value_pos - 3;
                    if( contains( node_level - 1, current.vertical_connections ) ) {
                        current.line.put_at( i, "╰" "─" "╴" );                                  //"┗" "━" "╸" );
                        current.vertical_connections.erase( node_level - 1 );
                    } else {
                        current.line.put_at( i, "╭" "─" "╴" );                                  //"┏" "━" "╸" );
                        current.vertical_connections.insert( node_level - 1 );
                    }
                }
                if( not is_empty( previous.line ) ) { out << previous.line.str() << endl; }
                previous = current;
            }
            if( not is_empty( previous.line ) ) { out << previous.line.str() << endl; }
        }
    };

    void display( in_<Binary_tree> tree ) { Displayer( tree, "...  " ).display(); }

    void run()
    {
        Binary_tree tree;
        #ifdef FULL_TREE_PLEASE
        {
            constexpr int max_n_values = intpow( 2, 5 );
            function<auto()->int> random_value = random::Integers( 1, 99, random::Seed( 3 ) );
            for( int i = 1; i <= max_n_values; ++i ) {
                tree.insert( random_value() );
            }
        }
        #else
        {
            for( const int value: {1, 6, 9, 4, 18, 15, 20, 21, 23} ) {
                tree.insert( value );
            }
        }
        #endif
        out << "Tree created:" << endl;
        display( tree );
    }
}  // namespace app

auto main() -> int { app::run(); }
