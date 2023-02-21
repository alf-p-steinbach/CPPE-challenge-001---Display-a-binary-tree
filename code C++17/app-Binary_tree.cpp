#include "app-Binary_tree.hpp"

#include <tuple>
#include <utility>

namespace app {
    using   cppu::popped_top_of;                        // "cpp_util.hpp"
    using   std::tie,                                   // <tuple>
            std::as_const, std::exchange, std::pair;    // <utility>

    void Binary_tree::insert_in( ref_<Node*> root, const int value )
    {
        if( root == nullptr ) {
            root = new Node{ value };
        } else {
            for( Node* p = root;; ) {
                if( value == p->value ) { return; }
                ref_<Node*> next_p = (value < p->value? p->left : p->right);
                if( next_p == nullptr ) { return insert_in( next_p, value ); }
                p = next_p;
            }
        }
    }

    void Binary_tree::Iterator::advance()
    {
        while( m_p_current != nullptr ) {
            if( not m_resume_from_found_node ) {
                if( m_p_previous == m_parents.top() ) {   // First time or after going down right.
                    while( m_p_current->left != nullptr ) {
                        m_parents.push( m_p_current );
                        tie( m_p_previous, m_p_current ) = pair( m_p_current, m_p_current->left );
                    }
                }

                if( m_p_previous != m_p_current->right ) {
                    m_resume_from_found_node = true;
                    break;
                }
            }
            m_resume_from_found_node = false;

            if( m_p_current->right != nullptr and m_p_previous != m_p_current->right ) {
                m_parents.push( m_p_current );
                tie( m_p_previous, m_p_current ) = pair( m_p_current, m_p_current->right );
            } else {
                tie( m_p_previous, m_p_current ) = pair( m_p_current, popped_top_of( m_parents ) );
            }
        }
    }

    Binary_tree::~Binary_tree()
    {
        if( m_root == nullptr ) { return; }

        stack<Node*> parents;
        parents.push( nullptr );
        for( Node* p = m_root; p != nullptr; ) {
            while( p->left != nullptr ) {
                parents.push( p );
                p = exchange( p->left, nullptr );
            }
            if( p->right == nullptr ) {
                delete p;
                p = popped_top_of( parents );
            } else {
                parents.push( p );
                p = exchange( p->right, nullptr );
            }
        }
    }
    
    auto Binary_tree::min_value() const
        -> int
    {
        Node* p = m_root;
        while( p->left != nullptr ) { p = p->left; }
        return p->value;
    }
    
    auto Binary_tree::max_value() const
        -> int
    {
        Node* p = m_root;
        while( p->right != nullptr ) { p = p->right; }
        return p->value;
    }
}  // namespace app
