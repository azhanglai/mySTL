#ifndef _UNINITIALIZED_H
#define _UNINITIALIZED_H

// 用于对未初始化空间构造元素

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace laistl {
    // uninitialized_copy: 把[first, last) 复制到 result 为起始处的空间，返回复制结束的位置
    template <class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
        ForwardIter result, std::true_type) 
    {
        return laistl::copy(first, last, result);
    }

    template <class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
        ForwardIter result, std::false_type) 
    {
        auto cur = result;
        try {
            for (; first != last; ++first, ++cur) {
                laistl::construct(&*cur, *first);
            }
        } catch (...) {
            for (; result != cur; --cur) {
                laistl::destroy(&*cur);
            }
        }
        return cur;
    }

    template <class InputIter, class ForwardIter>
    ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
        return laistl::unchecked_uninit_copy(first, last, result, 
                                             std::is_trivially_copy_assignable<
                                             typename iterator_traits<ForwardIter>::value_type>{});
    }

    // uninitialized_copy_n: 把[first, first + n) 复制到 result 为起始处的空间，返回复制结束的位置
    template <class InputIter, class Size, class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first, Size n,
        ForwardIter result, std::true_type) 
    {
        return laistl::copy_n(first, n, result).second;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first, Size n,
        ForwardIter result, std::false_type) 
    {
        auto cur = result;
        try {
            for (; n > 0; --n, ++first, ++cur) {
                laistl::construct(&*cur, *first);
            }
        } catch (...) {
            for (; result != cur; --cur) {
                laistl::destroy(&*cur);
            }
        }
        return cur;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
        return laistl::unchecked_uninit_copy_n(first, n, result, 
                                             std::is_trivially_copy_assignable<
                                             typename iterator_traits<InputIter>::value_type>{});
    }

    // uninitialized_fill: 把[first, last) 区间内填充元素值 
    template <class ForwardIter, class T>
    void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type) {
        laistl::fill(first, last, value);
    }

    template <class ForwardIter, class T>
    void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type) {
        auto cur = first;
        try {
            for (; cur != last; ++cur) {
                laistl::construct(&*cur, value);
            }
        } catch (...) {
            for (; first != cur; ++first) {
                laistl::destroy(&*first);
            }
        }
    }

    template <class ForwardIter, class T>
    void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
        laistl::unchecked_uninit_fill(first, last, value, 
                                      std::is_trivially_copy_assignable<
                                      typename iterator_traits<ForwardIter>::value_type>{});
    }

    // uninitialized_fill_n: 从first位置开始，填充n个元素值，返回填充结束的位置 
    template <class ForwardIter, class Size, class T>
    ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type) {
        return laistl::fill_n(first, n, value);
    }

    template <class ForwardIter, class Size, class T>
    ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type) {
        auto cur = first;
        try {
            for (; n > 0; --n, ++cur) {
                laistl::construct(&*cur, value);
            }
        } catch (...) {
            for (; first != cur; ++first) {
                laistl::destroy(&*first);
            }
        }
        return cur;
    }

    template <class ForwardIter, class Size, class T>
    ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
        laistl::unchecked_uninit_fill_n(first, n, value, 
                                      std::is_trivially_copy_assignable<
                                      typename iterator_traits<ForwardIter>::value_type>{});
    }

    // uninitialized_move: 把[first, last) 移动到 result 为起始处的空间，返回移动结束的位置
    template <class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_move(InputIter first, InputIter last,
        ForwardIter result, std::true_type) 
    {
        return laistl::move(first, last, result);
    }

    template <class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_move(InputIter first, InputIter last,
        ForwardIter result, std::false_type) 
    {
        ForwardIter cur = result;
        try {
            for (; first != last; ++first, ++cur) {
                laistl::construct(&*cur, laistl::move(*first));
            }
        } catch (...) {
            laistl::destroy(result, cur);
        }
        return cur;
    }

    template <class InputIter, class ForwardIter>
    ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result) {
        return laistl::unchecked_uninit_move(first, last, result, 
                                             std::is_trivially_move_assignable<
                                             typename iterator_traits<InputIter>::value_type>{});
    }

    // uninitialized_move_n: 把[first, first + n) 移动到 result 为起始处的空间，返回移动结束的位置
    template <class InputIter, class Size, class ForwardIter>
    ForwardIter unchecked_uninit_move_n(InputIter first, Size n,
        ForwardIter result, std::true_type) 
    {
        return laistl::move(first, first + n, result);
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter unchecked_uninit_move_n(InputIter first, Size n,
        ForwardIter result, std::false_type) 
    {
        auto cur = result;
        try {
            for (; n > 0; --n, ++first, ++cur) {
                laistl::construct(&*cur, laistl::move(*first));
            }
        } catch (...) {
            for (; result != cur; ++result) {
                laistl::destroy(result, cur);
            }
            throw;
        }
        return cur;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
        return laistl::unchecked_uninit_move_n(first, n, result, 
                                             std::is_trivially_move_assignable<
                                             typename iterator_traits<InputIter>::value_type>{});
    }


} /* namespace laistl */

#endif /*  _UNINITIALIZED_H */
