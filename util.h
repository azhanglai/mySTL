#ifndef _UTIL_H
#define _UTIL_H

// 通用工具，move, forward, swap, pair

#include <cstddef>
#include "type_traits.h"

namespace laistl {
    // move 
    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }
    
    // forward 
    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }

    // swap 
    template <class Tp>
    void swap(Tp& lhs, Tp& rhs) {
        auto tmp(laistl::move(lhs));
        lhs = laistl::move(rhs);
        rhs = laistl::move(tmp);
    }

    template <class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
        for (; first1 != last1; ++ first1, (void) ++first2) {
            laistl::swap(*first1, *first2);
        }
        return first2;
    }

    template <class Tp, size_t N>
    void swap(Tp(&a)[N], Tp(&b)[N]) {
        laistl::swap_range(a, a + N, b);
    }

    // pair 
    template <class K, class V>
    struct pair {
        using first_type = K;
        using second_type = V;
        
        first_type first;
        second_type second;

        // 默认构造函数 
        template <class Other1 = K, class Other2 = V, typename = typename std::enable_if<
            std::is_default_constructible<Other1>::value && 
            std::is_default_constructible<Other2>::value, void>::type>
        constexpr pair() : first(), second() {} 
        
        // 可隐式的有参构造函数(本类型) 
        template <class U1 = K, class U2 = V, typename std::enable_if<
            std::is_copy_constructible<U1>::value && 
            std::is_copy_constructible<U2>::value && 
            std::is_convertible<const U1&, K>::value && 
            std::is_convertible<const U2&, V>::value, int>::type = 0>
        constexpr pair(const K& a, const V& b) : first(a), second(b) {}        

        // 显示的有参数构造函数(本类型)
        template <class U1 = K, class U2 = V, typename std::enable_if<
            std::is_copy_constructible<U1>::value && 
            std::is_copy_constructible<U2>::value && 
            (!std::is_convertible<const U1&, K>::value ||
            !std::is_convertible<const U2&, V>::value), int>::type = 0>
        explicit constexpr pair(const K& a, const V& b) : first(a), second(b) {}        
    
        pair(const pair& rhs) = default;        // 拷贝构造
        pair(pair&& rhs) = default;             // 移动构造

        // 可隐式的有参构造函数(其他类型)
        template <class Other1, class Other2, typename std::enable_if<
            std::is_constructible<K, Other1>::value && 
            std::is_constructible<V, Other2>::value && 
            std::is_convertible<Other1&&, K>::value && 
            std::is_convertible<Other2&&, V>::value, int>::type = 0> 
        constexpr pair(Other1&& a, Other2&& b) 
            : first(laistl::forward<Other1>(a)), second(laistl::forward<Other2>(b)){} 

        // 显示的有参构造函数(其他类型)
        template <class Other1, class Other2, typename std::enable_if<
            std::is_constructible<K, Other1>::value && 
            std::is_constructible<V, Other2>::value && 
            (!std::is_convertible<Other1, K>::value || 
            !std::is_convertible<Other2, V>::value), int>::type = 0> 
        explicit constexpr pair(Other1&& a, Other2&& b) 
            : first(laistl::forward<Other1>(a)), second(laistl::forward<Other2>(b)){} 

        // 可隐式的拷贝构造函数(其他类型)
        template <class Other1, class Other2, typename std::enable_if<
            std::is_constructible<K, Other1&>::value && 
            std::is_constructible<V, Other2&>::value && 
            std::is_convertible<const Other1&, K>::value && 
            std::is_convertible<const Other2&, V>::value, int>::type = 0> 
        constexpr pair(const pair<Other1, Other2>& other) 
            : first(other.first), second(other.second){} 
        
        // 显示的拷贝构造函数(其他类型)
        template <class Other1, class Other2, typename std::enable_if<
            std::is_constructible<K, Other1&>::value && 
            std::is_constructible<V, Other2&>::value && 
            (!std::is_convertible<const Other1&, K>::value ||
            !std::is_convertible<const Other2&, V>::value), int>::type = 0> 
        explicit constexpr pair(const pair<Other1, Other2>& other) 
            : first(other.first), second(other.second){} 

        // 可隐式的移动构造函数(其他类型)
        template <class Other1, class Other2, typename std::enable_if<
            std::is_constructible<K, Other1>::value && 
            std::is_constructible<V, Other2>::value && 
            std::is_convertible<Other1, K>::value && 
            std::is_convertible<Other2, V>::value, int>::type = 0> 
        constexpr pair(pair<Other1, Other2>&& other) 
        : first(laistl::forward<Other1>(other.first)), second(laistl::forward<Other2>(other.second)){} 
        
        // 显示的移动构造函数(其他类型)
        template <class Other1, class Other2, typename std::enable_if<
            std::is_constructible<K, Other1>::value && 
            std::is_constructible<V, Other2>::value && 
            (!std::is_convertible<Other1, K>::value || 
            !std::is_convertible<Other2, V>::value), int>::type = 0> 
        explicit constexpr pair(pair<Other1, Other2>&& other) 
        : first(laistl::forward<Other1>(other.first)), second(laistl::forward<Other2>(other.second)){} 

        // 赋值运算符重载 
        pair& operator=(const pair& rhs) {
            if (this != &rhs) {
                first = rhs.first;
                second = rhs.second;
            }
            return *this;
        }

        pair& operator=(pair&& rhs) {
            if (this != &rhs) {
                first = laistl::move(rhs.first);
                second = laistl::move(rhs.second);
            }
            return *this;
        }

        template <class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>& other) {
            first = other.first;
            second = other.second;
            return *this;
        }

        template <class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>&& other) {
            first = laistl::forward<Other1>(other.first);
            second = laistl::forward<Other2>(other.second);
            return *this;
        }

        ~pair() = default;              // 析构函数
    
        void swap(pair& other) {
            if (this != &other) {
                laistl::swap(first, other.first);
                laistl::swap(second, other.second);
            }
        }

    };
    // 重载比较操作符 
    template <class K, class V>
    bool operator==(const pair<K, V>& lhs, const pair<K, V>& rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class K, class V>
    bool operator<(const pair<K, V>& lhs, const pair<K, V>& rhs) {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template <class K, class V>
    bool operator!=(const pair<K, V>& lhs, const pair<K, V>& rhs) {
        return !(lhs == rhs);
    }

    template <class K, class V>
    bool operator>(const pair<K, V>& lhs, const pair<K, V>& rhs) {
        return rhs < lhs;
    }

    template <class K, class V>
    bool operator<=(const pair<K, V>& lhs, const pair<K, V>& rhs) {
        return !(rhs < lhs);
    }
    
    template <class K, class V>
    bool operator>=(const pair<K, V>& lhs, const pair<K, V>& rhs) {
        return !(lhs < rhs);
    }
    // 重载swap
    template <class K, class V>
    void swap(pair<K, V>& lhs, pair<K, V>& rhs) {
        lhs.swap(rhs);
    }

    // 全局函数，让两个数据成为一个 pair 
    template <class K, class V>
    pair<K, V> make_pair(K&& first, V&& second) {
        return pair<K, V>(laistl::forward<K>(first), laistl::forward<V>(second));
    }

} /* namespace laistl */

#endif /* _UTIL_H */
