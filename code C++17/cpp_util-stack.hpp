#pragma once
#include <stack>            // std::stack

#include "cpp_util-type-builders.hpp"       // ref_

namespace cpp_util {
    template< class Item >
    inline auto popped_top_of( ref_<std::stack<Item>> st )
        -> Item
    {
        Item result = st.top();
        st.pop();
        return result;
    }
}  // namespace cpp_util
