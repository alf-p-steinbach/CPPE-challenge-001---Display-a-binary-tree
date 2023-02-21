#pragma once
#include <set>              // std::set

#include "cpp_util-type-builders.hpp"       // in_

namespace cpp_util {
    template< class Value >
    inline auto contains( in_<Value> v, in_<std::set<Value>> values )
        -> bool
    { return (values.find( v ) != values.end()); }
}  // namespace cpp_util
