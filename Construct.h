#ifndef __MINISTL_CONSTRUCT_H_
#define __MINISTL_CONSTRUCT_H_
#include <new>
#include "TypeTraits.h"
#include "Iterator.h"

namespace MiniStl
{
    template <class T1, class T2>
    inline void construct(T1 *ptr, const T2 &value)
    {
        new(ptr) T1(value);
    }

    template <class T>
    inline void destory(T *ptr)
    {
        ptr->~T();
    }
    
    // 对has_trival_destructor为true_type的对象不进行处理
    template <class ForwardIterator>
    inline void _destory(ForwardIterator first, ForwardIterator end, _true_type) {}

    template <class ForwardIterator>
    inline void _destory(ForwardIterator first, ForwardIterator end, _false_type)
    {
        for(; first != end; ++first)
            destory(&*first);
    }

    // 对destory分发
    template <class ForwardIterator, class T>
    inline void _destory_aux(ForwardIterator first, ForwardIterator end, T *)
    {
        typedef typename _type_traits<T>::has_trival_destructor has_trival_destructor;
        _destory(first, end, has_trival_destructor());
    }

    template <class ForwardIterator>
    inline void destory(ForwardIterator first, ForwardIterator end)
    {
        _destory_aux(first, end, value_type(first));
    }
    
    // destory第二版对char*和wchar_t*特例化
    template <>
    inline void destory(char * , char *) {}

    template <>
    inline void destory(wchar_t *, wchar_t *) {}
}
#endif
