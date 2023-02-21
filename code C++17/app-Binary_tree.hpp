#pragma once
#include "cpp_util.hpp"

#include <initializer_list>
#include <stack>

namespace app {
    namespace cppu = cpp_util;
    using   cppu::ref_, cppu::in_, cppu::int_size_of, cppu::Non_copyable;
    using   std::stack;         // <stack>
            
    class Binary_tree:
        public Non_copyable
    {
        struct Node
        {
            int         value;
            Node*       left;
            Node*       right;
        };

        static void insert_in( ref_<Node*> root, const int value );

        Node*   m_root  = nullptr;

    public:
        struct Node_info{ int value; int level; };

        class Iterator
        {
            friend class Binary_tree;
            using Node_stack = stack<Node*>;

            Node_stack      m_parents                   = Node_stack( {nullptr} );
            bool            m_resume_from_found_node    = false;
            Node*           m_p_previous                = nullptr;
            Node*           m_p_current;
            
            void advance();
            inline Iterator( Node* p_root = nullptr ): m_p_current( p_root ) { advance(); }

        public:
            inline auto operator*() const
                -> Node_info
            { return {m_p_current->value, int_size_of( m_parents ) - 1 }; }
            
            inline auto operator++()
                -> Iterator&
            { advance(); return *this; }
            
            friend auto operator==( in_<Iterator> a, in_<Iterator> b )
                -> bool
            { return (a.m_p_current == b.m_p_current); }

            friend auto operator!=( in_<Iterator> a, in_<Iterator> b )
                -> bool
            { return not(a == b); }
        };
    
        ~Binary_tree();

        inline auto begin() const -> Iterator   { return Iterator( m_root ); }
        inline auto end() const -> Iterator     { return Iterator(); }

        friend auto is_empty( in_<Binary_tree> tree ) -> bool { return not tree.m_root; }

        inline void insert( const int value )   { insert_in( m_root, value ); }

        auto min_value() const -> int;      // Precondition: not is_empty()
        auto max_value() const -> int;      // Precondition: not is_empty()
    };
}  // namespace app
