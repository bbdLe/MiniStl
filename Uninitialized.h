#ifndef __MINISTL_UNINITIALIZED_H
#define __MINISTL_UNINITIALIZED_H
#include"Algobase.h"
#include"TypeTraits.h"
#include"Iterator.h"
#include"Construct.h"

namespace MiniStl
{
    // unitializer_copy函数开始
    //重载
    template<class InputIterator, class OutputIterator>
    inline OutputIterator _uninitialized_copy_aux(InputIterator first, InputIterator end, OutputIterator result, _true_type)
    {
        return copy(first, end, result);
    }

    //重载
    template<class InputIterator, class OutputIterator>
    inline OutputIterator _uninitialized_copy_aux(InputIterator first, InputIterator end, OutputIterator result, _false_type)
    {
        for(; first != end; ++first, ++result)
        {
            construct(&*result, *first);
        }
        return result;
    }

    template<class InputIterator, class OutputIterator, class T>
    inline OutputIterator _uninitialized_copy(InputIterator first, InputIterator end, OutputIterator result, T*)
    {
        typedef typename _type_traits<T>::is_POD_type is_POD_type;
        return _uninitialized_copy_aux(first, end, result, is_POD_type());
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator uninitialized_copy(InputIterator first, InputIterator end, OutputIterator result)
    {
        return _uninitialized_copy(first, end, result, value_type(result));
    }
    // uninitialized_copy函数结束

    // _uninitialized_copy_copy函数
    template<class InputIterator1, class InputIterator2, class OutputIterator>
    inline OutputIterator _uninitialized_copy_copy(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        result = uninitialized_copy(first1, last1, result);
        return uninitialized_copy(first2, last2, result);
    }

    // uninitialized_fill_n函数开始
    template<class OutputIterator, class Size, class T>
    inline OutputIterator _uinitialized_fill_n_aux(OutputIterator result, Size n, const T& value, _false_type)
    {
        for(; n > 0; ++n, ++result)
        {
            construct(&*result, value);
        }
        return result;
    }

    template<class OutputIterator, class Size, class T>
    inline OutputIterator _uninitialized_fill_n_aux(OutputIterator result, Size n, const T& value, _true_type)
    {
        return fill_n(result, n, value);
    }

    template<class OutputIterator, class Size, class T, class T1>
    inline OutputIterator _uninitialized_fill_n(OutputIterator result, Size n, const T& value, T1*)
    {
        typedef typename _type_traits<T1>::is_POD_type is_POD_type;
        return _uninitialized_fill_n_aux(result, n, value, is_POD_type());
    }

    template<class OutputIterator, class Size, class T>
    inline OutputIterator uninitialized_fill_n(OutputIterator result, Size n, const T& value)
    {
        return _uninitialized_fill_n(result, n, value, value_type(result));
    }
    // uninitialized_fill_n函数结束

    // uninitialized_fill函数
    template<class OutputIterator, class T>
    inline OutputIterator _uninitialized_fill_aux(OutputIterator first, OutputIterator end, const T& value, _false_type)
    {
        for(;first != end; ++first)
        {
            construct(&*first, value);
        }
        return first;
    }

    template<class OutputIterator, class T>
    inline OutputIterator _uninitialized_fill_aux(OutputIterator first, OutputIterator end, const T& value, _true_type)
    {
        return fill(first, end, value);
    }

    template<class OutputIterator, class T, class T1>
    inline OutputIterator _uninitialized_fill(OutputIterator first, OutputIterator end, const T& value, T1*)
    {
        typedef typename _type_traits<T1>::is_POD_type is_POD_type;
        return _uninitialized_fill_aux(first, end, value, is_POD_type());
    }

    template<class OutputIterator, class T>
    inline OutputIterator uninitialized_fill(OutputIterator first, OutputIterator end, const T& value)
    {
        return _uninitialized_fill(first, end, value, value_type(first));
    }
}
#endif
