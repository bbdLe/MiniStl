#ifndef __MINISTL_ALLOCATOR_H
#define __MINISTL_ALLOCATOR_H
#include "Construct.h"
#include "Alloc.h"
#include<cstddef>
#include<new>

namespace MiniStl
{
    template <class T>
    class Allocator
    {
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef ptrdiff_t difference_type;
            typedef size_t size_type;

            static T* allocate();
            static T* allocate(size_t n);
            static void deallocate(T* ptr);
            static void deallocate(T* ptr, size_t n);
            static void construct(T* ptr);
            static void construct(T* ptr, const T& value);
            static void destory(T* ptr);
            static void destory(T* first, T* end);
    };

    template <class T>
    T* Allocator<T>::allocate()
    {
        return static_cast<T *>(myAlloc::allocate(sizeof(T)));
    }

    template <class T>
    T* Allocator<T>::allocate(size_t n)
    {
        if(n == 0) return static_cast<T*>(NULL);
        return static_cast<T *>(myAlloc::allocate(sizeof(T) * n));
    }

    template <class T>
    void Allocator<T>::deallocate(T* ptr)
    {
        myAlloc::deallocate(static_cast<void *>(ptr), sizeof(T));
    }

    template <class T>
    void Allocator<T>::deallocate(T* ptr, size_t n)
    {
        if(n == 0) return;
        myAlloc::deallocate(static_cast<void *>(ptr), sizeof(T) * n);
    }

    template <class T>
    void Allocator<T>::construct(T *ptr)
    {
        new(static_cast<void *>(ptr)) T();
    }

    template <class T>
    void Allocator<T>::construct(T *ptr, const T &value)
    {
        new(static_cast<void *>(ptr)) T(value);
    }

    template <class T>
    void Allocator<T>::destory(T *ptr)
    {
        ptr->~T();
    }

    template <class T>
    void Allocator<T>::destory(T *first, T *end)
    {
        for(; first != end; ++first)
            first->~T();
    }

}

#endif
