#ifndef __MINISTL_ALGORITHM_H
#define __MINISTL_ALGORITHM_H

#include"Algobase.h"
#include"Algo.h"
#include"Functional.h"
#include<iostream>

namespace MiniStl
{
    const int threadhold = 16;

    template<class BidirectionalIterator>
    bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
    {
        if(first == last)
            return false;
        BidirectionalIterator i = first;
        ++i;
        if(i == last)
            return false;
        i = last;
        --i;
        while(true)
        {
            BidirectionalIterator ii = i;
            --i;
            if(*i < *ii)
            {
                BidirectionalIterator j = last;
                while(!(*i < *--j));
                iter_swap(i, j);
                reverse(ii, last);
                return true;
            }
            else if(i == first)
            {
                reverse(first, last);
                return false;
            }
        }
    }

    template<class BidirectionalIterator, class Compare>
    bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
    {
        if(first == last)
            return false;
        BidirectionalIterator i = first;
        ++i;
        if(i == last)
            return false;
        i = last;
        --i;
        while(true)
        {
            BidirectionalIterator ii = i;
            --i;
            if(comp(*i, *ii))
            {
                BidirectionalIterator j = last;
                while(!comp(*i, *--j));
                iter_swap(i, j);
                reverse(ii, last);
                return true;
            }
            else if(i == first)
            {
                reverse(first, last);
                return false;
            }
        }
    }

    template<class BidirectionalIterator>
    bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
    {
        if(first == last)
            return false;
        BidirectionalIterator i = first;
        ++i;
        if(i == last)
            return false;
        i = last;
        --i;
        while(true)
        {
            BidirectionalIterator ii = i;
            --i;
            if(*ii < *i)
            {
                BidirectionalIterator j = last;
                while(!(*--j < *i));
                iter_swap(i, j);
                reverse(ii, last);
                return true;
            }
            else if(i == first)
            {
                reverse(first, last);
                return false;
            }
        }
    }

    template<class BidirectionalIterator, class Compare>
    inline bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
    {
        if(first == last)
            return false;
        BidirectionalIterator i = first;
        ++i;
        if(i == last)
            return false;
        i = last;
        --i;
        while(true)
        {
            BidirectionalIterator ii = i;
            --i;
            if(comp(*ii, *i))
            {
                BidirectionalIterator j = last;
                while(!comp(*--j, *i));
                iter_swap(j, i);
                reverse(ii, last);
                return true;
            }
            else if(first == i)
            {
                reverse(first, last);
                return false;
            }
        }
    }

    template<class RandomIterator>
    void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last)
    {
        typedef typename _iterator_traits<RandomIterator>::difference_type difference_type;
        typedef typename _iterator_traits<RandomIterator>::value_type value_type;
        
        make_heap(first, middle);

        for(RandomIterator iter = middle; iter < last; ++iter)
        {
            if(*iter < *first)
            {
                iter_swap(iter, first);
                _heapify(first, middle, less<value_type>(), difference_type(0), middle - first, static_cast<value_type*>(NULL));
            }
        }
        heap_sort(first, middle);
    }

    template<class RandomIterator, class Compare>
    void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last, Compare comp)
    {
        typedef typename _iterator_traits<RandomIterator>::difference_type difference_type;
        typedef typename _iterator_traits<RandomIterator>::value_type value_type;

        make_heap(first, middle, comp);
        for(RandomIterator iter = middle; iter != last; ++iter)
        {
            if(comp(*iter, *first))
            {
                iter_swap(first, iter);
                _heapify(first, middle, comp, difference_type(0), middle - first, static_cast<value_type*>(NULL));
            }
        }
        heap_sort(first, middle);
    }

    template<class InputIterator, class RandomIterator>
    void partial_sort_copy(InputIterator first, InputIterator last, RandomIterator result_first, RandomIterator result_last)
    {
        typedef typename _iterator_traits<InputIterator>::value_type input_value_type;
        typedef typename _iterator_traits<RandomIterator>::difference_type difference_type;
        typedef typename _iterator_traits<RandomIterator>::value_type result_value_type;

        if(result_first == result_last)
            return;
        
        for(RandomIterator iter = result_first; iter != result_last; ++iter, ++first)
        {
            *iter = *first;
        }

        make_heap(result_first, result_last);

        for(; first != last; ++first)
        {
            input_value_type temp = *first;
            if(temp < *result_first)
            {
                *result_first = temp;
                _heapify(result_first, result_last, less<result_value_type>(), difference_type(0), result_last - result_first, static_cast<result_value_type*>(NULL));
            }
        }

        heap_sort(result_first, result_last);
    }

    template<class InputIterator, class RandomIterator, class Compare>
    void partial_sort_copy(InputIterator first ,InputIterator last, RandomIterator result_first, RandomIterator result_last, Compare comp)
    {
        typedef typename _iterator_traits<InputIterator>::value_type input_value_type;
        typedef typename _iterator_traits<RandomIterator>::value_type output_value_type;
        typedef typename _iterator_traits<RandomIterator>::difference_type difference_type;

        if(result_first == result_last)
            return;

        for(RandomIterator iter = result_first; iter != result_last; ++iter, ++first)
        {
            *iter = *first;
        }

        make_heap(result_first, result_last, comp);
        for(; first != last; ++first)
        {
            input_value_type temp = *first;
            if(comp(temp, *result_first))
            {
                *result_first = temp;
                _heapify(result_first, result_last, comp, difference_type(0), result_last - result_first, static_cast<output_value_type*>(NULL));
            }
        }

        heap_sort(result_first, result_last);
    }

    template<class RandomIterator, class T>
    inline void __unguarded_liner_insert(RandomIterator last, const T value)
    {
        RandomIterator prev = last - 1;
        while(value < *prev)
        {
            *last = *prev;
            last = prev;
            --prev;
        }

        *last = value;
    }

    template<class RandomIterator, class T>
    inline void __liner_insert(RandomIterator first, RandomIterator last, T*)
    {
        T temp = *last;

        if(temp < *first)
        {
            copy_backward(first, last, last + 1);
            *first = temp;
        }
        else
            __unguarded_liner_insert(last, temp);
    }

    template<class RandomIterator>
    inline void __insertion_sort(RandomIterator first, RandomIterator last)
    {
        if(first == last)
            return;
        for(RandomIterator i = first + 1; i != last; ++i)
        {
            __liner_insert(first, i, value_type(first));
        }
    }

    template<class T>
    inline const T& __median(const T& a, const T& b, const T& c)
    {
        if(a < b)
            if(b < c)   // a < b < c
                return b;
            else if(a > c)      // a < b, b >= c, a > c
                return a;
            else
                return c;       // a < b, b >= c, a <= c
        else if(a < c)  // a < c, a >= b
            return a;
        else if(b < c)  // a >=b, a >=c, c > b
            return c;
        else        // a >= b, a >= c, b >= c
            return b;
    }


    //
    template<class RandomIterator, class T>
    RandomIterator __unguarded_partition(RandomIterator first, RandomIterator last, T privot)
    {
        while(true)
        {
            while(*first < privot)
                ++first;
            --last;
            while(privot < *last)
                --last;
            if(!(first < last)) return first;
            iter_swap(first, last);
            ++first;
        }
    }

    template<class RandomIterator, class T>
    inline void __unguarded_insertion_aux(RandomIterator first, RandomIterator last, T*)
    {
        for(; first != last; ++first)
            __unguarded_liner_insert(first, *first);
    }

    template<class RandomIterator>
    inline void __unguarded_insertion_sort(RandomIterator first, RandomIterator last)
    {
        __unguarded_insertion_aux(first, last, value_type(first));
    }

    template<class RandomIterator>
    inline void __final_insertion_sort(RandomIterator first, RandomIterator last)
    {
        if(last - first > threadhold)
        {
            __insertion_sort(first, first + threadhold);
            __unguarded_insertion_sort(first + threadhold, last);
        }
        else
        {
            __insertion_sort(first, last);
        }
    }

    template<class RandomIterator>
    inline void __quick_sort_loop(RandomIterator first, RandomIterator last)
    {
        while(last - first > threadhold)
        {
            RandomIterator cut = __unguarded_partition(first, last, __median(*first, *(first + (last - first) / 2), *(last - 1)));

            if(cut - first < last - cut)
            {
                __quick_sort_loop(first, cut);
                first = cut;
            }
            else
            {
                __quick_sort_loop(cut, last);
                last = cut;
            }
        }
    }


    template<class RandomIterator>
    inline void sort(RandomIterator first, RandomIterator last)
    {
        if(first != last)
        {
            __quick_sort_loop(first, last);
            __final_insertion_sort(first, last);
        }
    }

    template<class RandomIterator, class Distance, class T>
    inline pair<RandomIterator, RandomIterator> __equal_range(RandomIterator first, RandomIterator last, const T& value, Distance *, random_iterator_tag)
    {
        Distance len = last - first;
        Distance mid;

        while(len > 0)
        {
            mid = len >> 1;
            RandomIterator iter = first + mid;
            if(*iter < value)
            {
                len -= mid + 1;
                first = iter + 1;
            }
            else if(value < *iter)
                len = mid;
            else
            {
                first = lower_bound(first, iter, value);
                last = upper_bound(iter + 1, first + len, value);
                return make_pair(first, last);
            }
        }

        return make_pair(first, last);
    }

    template<class ForwardIterator, class Distance, class T>
    inline pair<ForwardIterator, ForwardIterator> __equal_range(ForwardIterator first, ForwardIterator last, const T& value, Distance *, forward_iterator_tag)
    {
        Distance len = distance(first, last);
        Distance mid;

        while(len > 0)
        {
            mid = len >> 1;
            ForwardIterator iter = first;
            advance(iter, mid);
            if(*iter < value)
            {
                len -= mid + 1;
                first = iter;
                ++first;
            }
            else if(value < *iter)
            {
                len = mid;
            }
            else
            {
                first = lower_bound(first, iter, value);
                last = upper_bound(++iter, first + len, value);
                return make_pair(first, last);
            }
        }

        return make_pair(first, first);
    }

    template<class ForwardIterator, class T>
    inline pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value)
    {
        return __equal_range(first, last, value, difference_type(first), iterator_category(first));
    }

    template<class RandomIterator>
    inline void nth_element(RandomIterator first, RandomIterator nth, RandomIterator last)
    {
        if(nth == last)
            return;

        while(last - first > 3)
        {
            RandomIterator pos = __unguarded_partition(first, last, __median(*first, *(first + (last - first) / 2), *(last - 1)));

            if(nth < pos)
                last = pos;
            else
                first = pos;
        }

        __insertion_sort(first, last);
    }
    
}

#endif
