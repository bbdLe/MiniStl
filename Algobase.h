#ifndef __MINISTL_ALGOBASE_H
#define __MINISTL_ALGOBASE_H
#include<cstring>
#include<functional>
#include<iostream>
#include"Iterator.h"
#include"TypeTraits.h"
#include"Utility.h"
#include"Functional.h"

namespace MiniStl
{
    // fill_n函数
    template <class OutputIterator, class Size, class T>
    inline OutputIterator fill_n(OutputIterator result, Size n, const T& value)
    {
        for(; n > 0; --n, ++result)
            *result = value;
        return result;
    }

    // fill函数
    template <class OutputIterator, class T>
    inline OutputIterator fill(OutputIterator first, OutputIterator end, const T& value)
    {
        for(; first != end; ++first)
        {
            *first = value;
        }
        return first;
    }

    // copy函数

    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy_d(InputIterator first, InputIterator end, OutputIterator result)
    {
        typedef typename _iterator_traits<InputIterator>::difference_type Distance;
        Distance n = end - first;
        for(; n != 0; --n)
        {
            *result = *first;
            ++result;
            ++first;
        }
        return result;
    }

    template<class InputIterator, class OutputIterator>
    struct _copy_dispatch;

    template<class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator second, OutputIterator result)
    {
        return _copy_dispatch<InputIterator, OutputIterator>()(first, second, result);
    }

    //重载版本
    inline char* copy(const char *first, const char *end, char *result)
    {
        //保证重叠部分会被安全处理, 不可以用memcpy
        std::memmove(result, first, end - first);
        return result + (end - first);
    }

    //重载版本
    inline wchar_t* copy(const wchar_t *first, const wchar_t *end, wchar_t *result)
    {
        //保证重叠部分会被安全处理, 不可以用memcpy
        std::memmove(result, first, end - first);
        return result + (end - first);
    }

    template<class InputIterator, class OutputIterator>
    struct _copy_dispatch
    {
        OutputIterator operator()(InputIterator first, InputIterator end, OutputIterator result)
        {
            return _copy(first, end, result, iterator_category(first));
        }
    };

    // 对指针偏例化
    template<class T>
    struct _copy_dispatch<T*, T*>
    {
        T* operator()(T* first, T* end, T* result)
        {
            return _copy_t(first, end, result, typename _type_traits<T>::has_trival_assignment_operator());
        }
    };

    // 对指针偏例化
    template<class T>
    struct _copy_dispatch<const T*, T*>
    {
        T* operator()(const T* first, const T* end, T* result)
        {
            
            return _copy_t(first, end, result, typename _type_traits<T>::is_POD_type());
        }
    };

    // 对一般迭代器
    template<class InputIterator, class OutputIterator>
    OutputIterator _copy(InputIterator first, InputIterator end, OutputIterator result, input_iterator_tag)
    {
        for(;first != end; ++end, ++result)
        {
            *result = *first;
        }
        return result;
    }

    // 对于随机迭代器
    template<class InputIterator, class OutputIterator>
    OutputIterator _copy(InputIterator first, InputIterator end, OutputIterator result, random_iterator_tag)
    {
        return _copy_d(first, end, result);
    }

/*  template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy_d(InputIterator first, InputIterator end, OutputIterator result)
    {
        typedef typename _iterator_traits<InputIterator>::difference_type Distance;
        Distance n = end - first;
        for(; n != 0; --n)
        {
            *result = *first;
            ++result;
            ++first;
        }
        return result;
    }
*/
    template<class T>
    inline T* _copy_t(const T* first, const T* end, T* result, _true_type)
    {
        std::memmove(result, first, (end - first) * sizeof(T));
        return result + (end - first);
    }

    template<class T>
    inline T* _copy_t(const T* first, const T* end, T* result, _false_type)
    {
        return _copy_d(first, end, result);
    }
    //copy函数结束

    //copy_backward函数
    template<class BirdirectionalIterator1, class BirdirectionalIterator2>
    struct copy_backward_dispatch
    {
        BirdirectionalIterator2 operator()(BirdirectionalIterator1 first, BirdirectionalIterator1 end, BirdirectionalIterator2 result)
        {
            return _copy_backward(first, end, result, iterator_category(first));
        }
    };  
    
    // 指针特殊情况
    template<class T>
    inline T* copy_backward_t(const T* first, const T* end, T *result, _true_type)
    {
        ptrdiff_t n = end - first;
        std::memmove(result - n, first, n * sizeof(T));
        return result - n;
    }

    template<class T>
    inline T* copy_backward_t(const T* first, const T* end, T *result, _false_type)
    {
        return copy_backward_d(first, end, result);
    }

    // 对指针类型偏例化
    template<class T>
    struct copy_backward_dispatch<T *, T *>
    {
        T* operator()(T *first, T *end, T *result)
        {
            typedef typename _type_traits<T>::has_trival_assignment_operator has_trival_assignment_operator;
            return copy_backward_t(first, end, result, has_trival_assignment_operator());
        }
    };

    template<class T>
    struct copy_backward_dispatch<const T*, T*>
    {
        T* operator()(const T* first, const T* end, T *result)
        {
            typedef typename _type_traits<T>::has_trival_assignment_operator has_trival_assignment_operator;
            return copy_backward_t(first, end, result, has_trival_assignment_operator());
        }
    };

    // 随机迭代器共享函数
    template<class BirdirectionalIterator1, class BirdirectionalIterator2>
    inline BirdirectionalIterator2 _copy_backward_d(BirdirectionalIterator1 first, BirdirectionalIterator1 end, BirdirectionalIterator2 result)
    {
        typedef typename _iterator_traits<BirdirectionalIterator1>::difference_type Distance;
        Distance n = end - first;
        while(n--)
            *(--result) = *(--end);
        return result;
    }

    // 随机迭代器优化
    template<class BirdirectionalIterator1, class BirdirectionalIterator2>
    inline BirdirectionalIterator2 _copy_backward(BirdirectionalIterator1 first, BirdirectionalIterator1 end, BirdirectionalIterator2 result, random_iterator_tag)
    {
        return _copy_backward_d(first, end, result);
    }

    // 双向迭代器
    template<class BirdirectionalIterator1, class BirdirectionalIterator2>
    inline BirdirectionalIterator2 _copy_backward(BirdirectionalIterator1 first, BirdirectionalIterator1 end, BirdirectionalIterator1 result, bidirectional_iterator_tag)
    {
        while(first != end)
            *(--result) = *(--end);
        return result;
    }

    template<class BirdirectionalIterator1, class BirdirectionalIterator2>
    inline BirdirectionalIterator2 copy_backward(BirdirectionalIterator1 first, BirdirectionalIterator1 end, BirdirectionalIterator2 result)
    {
        return copy_backward_dispatch<BirdirectionalIterator1, BirdirectionalIterator2>()(first, end, result);
    }

    // make_heap函数
    // I think the argument len can remove:)
    template<class RandomIterator, class Compare, class Distance, class T>
    void _heapify(RandomIterator first, RandomIterator last, Compare comp, Distance offset, Distance len, T*)
    {
        Distance leftChild = offset * 2 + 1;
        Distance rightChild = offset * 2 + 2;
        Distance maxOffset = offset;
        
        while(leftChild < len)
        {
            if(comp(*(first + maxOffset), *(first + leftChild)))
                maxOffset = leftChild;
            if(rightChild < len && comp(*(first + maxOffset), *(first + rightChild)))
                maxOffset = rightChild;
            if(maxOffset != offset)
            {
                using MiniStl::swap;
                swap(*(first + offset), *(first + maxOffset));
                offset = maxOffset;
                leftChild = offset * 2 + 1;
                rightChild = offset * 2 + 2;
            }
            else
                break;
        }
    }

    template<class RandomIterator, class Compare = std::less<typename _iterator_traits<RandomIterator>::value_type>>
    void make_heap(RandomIterator first, RandomIterator last, Compare comp = Compare())
    {
        typedef typename _iterator_traits<RandomIterator>::difference_type Distance;
        Distance len = distance(first, last);
        if(len == 1)
            return;

        for(Distance offset = (len - 2) / 2; offset >= 0; --offset)
            _heapify(first, last, comp, offset, len, value_type(first));
    }

    // push_heap函数
    template<class RandomIterator, class Compare = std::less<typename _iterator_traits<RandomIterator>::value_type>>
    void push_heap(RandomIterator first, RandomIterator last, Compare comp = Compare())
    {
        if(last - first <= 1)
            return;

        typedef typename _iterator_traits<RandomIterator>::difference_type Distance;
        typedef typename _iterator_traits<RandomIterator>::value_type value_type;
        Distance self = distance(first, last - 1);
        Distance parent = (self - 1) / 2;
        value_type temp = *(first + self);  // keep this value becaues we may use it a lot of times

        while(self > 0)
        {
            if(comp(*(first + parent), temp))
            {
                *(first + self) = *(first + parent);
                self = parent;
                parent = (self - 1) / 2;
            }
            else
                break;
        }

        *(first + self) = temp;
    }

    // pop_heap函数
    template<class RandomIterator, class Compare = std::less<typename _iterator_traits<RandomIterator>::value_type>>
    void pop_heap(RandomIterator first, RandomIterator last, Compare comp = Compare())
    {
        typedef typename _iterator_traits<RandomIterator>::difference_type Distance;
        Distance len = distance(first, last);

        using MiniStl::swap;
        swap(*first, *(last - 1));
        if(len - 1 != 0)
            _heapify(first, last - 1, comp, static_cast<Distance>(0), len - 1, value_type(first));
    }

    // heap_sort函数
    template<class RandomIterator, class Compare = MiniStl::less<typename _iterator_traits<RandomIterator>::value_type>>
    void heap_sort(RandomIterator first, RandomIterator last, Compare comp = Compare())
    {
        typedef typename _iterator_traits<RandomIterator>::difference_type Distance;
        
        make_heap(first, last, comp);
        Distance n = 0;
        for(Distance len = distance(first, last); len != 1; --len, ++n)
            pop_heap(first, last - n, comp);
    }

    // lower_bound函数
    template<class RandomIterator, class T, class Distance>
    RandomIterator _lower_bound(RandomIterator first, RandomIterator last, const T& value, Distance*, random_iterator_tag)
    {
        Distance len = last - first;

        while(len > 0)
        {
            Distance n = len >> 1;
            RandomIterator mid = first + n;
            if(*mid < value)
            {
                first = mid + 1;
                len -= n + 1;
            }
            else
            {
                len = n;
            }
        }

        return first;
    }

    template<class ForwardIterator, class T, class Distance>
    ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
    {
        Distance len = distance(first, last);

        while(len > 0)
        {
            Distance n = len >> 1;
            ForwardIterator mid = first;
            advance(first, n);
            if(*mid < value)
            {
                first = mid;
                ++first;
                len -= n + 1;
            }
            else
            {
                len = n;
            }
        }

        return first;
    }

    template<class ForwardIterator, class T, class Compare, class Distance>
    ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp, Distance*, forward_iterator_tag)
    {
        Distance len = distance(first, last);

        while(len > 0)
        {
            Distance n = len >> 1;
            ForwardIterator mid = first;
            advance(mid, n);
            if(comp(*mid, value))
            {
                first = mid;
                ++first;
                len -= n + 1;
            }
            else
            {
                len = n;
            }
        }

        return first;
    }

    template<class RandomIterator, class T, class Compare, class Distance>
    RandomIterator _lower_bound(RandomIterator first, RandomIterator last, const T& value, Compare comp, Distance*, random_iterator_tag)
    {
        Distance len = last - first;

        while(len > 0)
        {
            Distance n = len >> 1;
            RandomIterator mid = first + n;
            if(comp(*mid, value))
            {
                first = mid + 1;
                len -= n + 1;
            }
            else
            {
                len = n;
            }
        }

        return first;
    }

    template<class ForwardIterator, class T>
    ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value)
    {
        return _lower_bound(first, last, value, difference_type(first), iterator_category(first));
    }

    template<class ForwardIterator, class T, class Compare>
    ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
    {
        return _lower_bound(first, last, value, comp, difference_type(first), iterator_category(first));
    }
    // upper_bound函数
    template<class ForwardIterator, class T, class Distance>
    ForwardIterator _upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
    {
        Distance len = distance(first, last);

        while(len > 0)
        {
            Distance n = len >> 1;
            ForwardIterator mid = first;
            advance(mid, n);
            if(value < *mid)
            {
                len = n;
            }
            else
            {
                len -= n + 1;
                first = mid;
                ++first;
            }
        }

        return first;
    }

    template<class RandomIterator, class T, class Distance>
    RandomIterator _upper_bound(RandomIterator first, RandomIterator last, const T& value, Distance*, random_iterator_tag)
    {
        Distance len = last - first;

        while(len > 0)
        {
            Distance n = len >> 1;
            RandomIterator mid = first + n;
            if(value < *mid)
            {
                len = n;
            }
            else
            {
                len -= n + 1;
                first = mid + 1;
            }
        }

        return first;
    }

    template<class ForwardIterator, class T, class Compare, class Distance>
    ForwardIterator _upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp, Distance*, forward_iterator_tag)
    {
        Distance len = distance(first, last);

        while(len > 0)
        {
            Distance n = len >> 1;
            ForwardIterator mid = first;
            advance(first, n);
            if(comp(value, *mid))
            {
                len = n;
            }
            else
            {
                len -= n + 1;
                first = mid;
                ++first;
            }
        }

        return first;
    }

    template<class ForwardIterator, class T>
    ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value)
    {
        return _upper_bound(first, last, value, difference_type(first), iterator_category(first));
    }

    template<class ForwardIterator, class T, class Compare>
    ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
    {
        return _upper_bound(first, last, value, comp, difference_type(first), iterator_category(first));
    }

    // equal_range
    /*template<class ForwardIterator, class T>
    pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value)
    {
        return make_pair(lower_bound(first, last, value), upper_bound(first, last, value));
    }*/

    /*template<class ForwardIterator, class T, class Compare>
    pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
    {
        return make_pair(lower_bound(first, last, value, comp), upper_bound(first, last, value, comp));
    }*/

    template<class InputIterator1, class InputIterator2>
    pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
    {
        while(first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }

        return make_pair(first1, first2);
    }

    template<class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
    {
        while(first1 != last1)
        {
            if(*first1 != *first2)
                return false;
            else
            {
                ++first1;
                ++first2;
            }
        }

        return true;
    }

    template<class ForwardIterator>
    void iter_swap(ForwardIterator iter1, ForwardIterator iter2)
    {
        typename _iterator_traits<ForwardIterator>::value_type temp = *iter1;
        *iter1 = *iter2;
        *iter2 = temp;
    }

    template<class T>
    const T& max(const T& a, const T& b)
    {
        return a < b? b : a;
    }

    template<class T>
    const T& min(const T& a, const T& b)
    {
        return a < b? a : b;
    }

    template<class InputIterator1, class InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
                return true;
            else if(*first1 > *first2)
                return false;
            else
            {
                ++first1;
                ++first2;
            }
        }

        return first1 == last1 && first2 != last2;
    }

    template<class InputIterator1, class InputIterator2, class Compare>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first1, *first2))
                return true;
            else if(comp(*first2, *first1))
                return false;
            else
            {
                ++first1;
                ++first2;
            }
        }

        return first1 == last1 && first2 != last2;
    }

    template<>
    bool lexicographical_compare(const char* first1, const char* last1, const char* first2, const char* last2)
    {
        ptrdiff_t len1 = last1 - first1;
        ptrdiff_t len2 = last2 - first2;

        int result = strncmp(first1, first2, min(len1, len2));

        return result < 0? true : result == 0? len1 < len2 : false;
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
            {
                *result = *first1;
                ++first1;
            }
            else if(*first2 < *first1)
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }

        return MiniStl::copy(first2, last2, MiniStl::copy(first1, last1, result));
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first1, *first2))
            {
                *result = *first1;
                ++first1;
            }
            else if(comp(*first2, *first1))
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }

        return MiniStl::copy(first2, last2, MiniStl::copy(first1, last1, result));
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
                ++first1;
            else if(*first2 < *first1)
                ++first2;
            else
            {
                result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }

        return result;
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first1, *first2))
                ++first1;
            else if(comp(*first2, *first1))
                ++first2;
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }

        return result;
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if(*first2 < *first1)
                ++first2;
            else
            {
                ++first1;
                ++first2;
            }
        }
        return MiniStl::copy(first1, last1, result);
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first1, *first2))
            {
                *result = *first1;
                ++result;
                ++first1;
            }
            else if(comp(*first2, *first1))
            {
                *result = *first2;
                ++result;
                ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }

        return MiniStl::copy(first1, last1, result);
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
            {
                *result = *first1;
                ++result;
                ++first1;
            }
            else if(*first2 < *first1)
            {
                *result = *first2;
                ++result;
                ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return MiniStl::copy(first2, last2, MiniStl::copy(first1, last1, result));
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first1, *first2))
            {
                *result = *first1;
                ++result;
                ++first1;
            }
            else if(comp(*first2, *first1))
            {
                *result = *first2;
                ++result;
                ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }

        return MiniStl::copy(first2, last2, MiniStl::copy(first1, last1, result));
    }

    template<class ForwardIterator, class T>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T& value)
    {
        first = lower_bound(first, last, value);

        return first != last && !(value < *first);
    }

    template<class ForwardIterator, class T, class Compare>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
    {
        first = lower_bound(first, last, value, comp);

        return first != last && !comp(value, *first);
    }


}
#endif
