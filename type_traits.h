#ifndef _TYPE_TRAITS_H
#define _TYPE_TRAITS_H

// 提取类型信息

#include <type_traits>

namespace laistl {
    template <class T, T v>
    struct m_integral_constant {
        static constexpr T value = v;  
    };

    template <bool B>
    using m_bool_constant = m_integral_constant<bool, B>;
        
    using m_true_type = m_bool_constant<true>;
    using m_false_type = m_bool_constant<false>;

    // is_pair
    template <class K, class V>
    struct pair;

    template <class T>
    struct is_pair : laistl::m_false_type {};

    template <class K, class V>
    struct is_pair<laistl::pair<K, V>> : laistl::m_true_type {};
    
} /* namespace laistl */

#endif  /* _TYPE_TRAITS_H */

