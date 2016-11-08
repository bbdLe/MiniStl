#ifndef __MINISTL_ALGO_H
#define __MINISTL_ALGO_H

#include"Iterator.h"
#include"Algobase.h"
#include<iostream>

namespace MiniStl
{

    template<class ForwardIterator>
    inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
    {
        if(first == last)
            return last;
        else
        {
            ForwardIterator next = first;
            ++next;
            while(next != last)
            {
                if(*next == *first)
                    return first;
                else
                {
                    first = next;
                    ++next;
                }   
            }

            return last;
        }
    }

    template<class ForwardIterator, class BinaryPredicate>
    inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)
    {
        if(first == last)
            return last;
        else
        {
            ForwardIterator next = first;
            ++next;
            while(next != last)
            {
                if(pred(*first, *next))
                    return first;
                else
                {
                    first = next;
                    ++next;
                }
            }

            return last;
        }
    }

    template<class InputIterator, class T>
    inline typename _iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& value)
    {
        typename _iterator_traits<InputIterator>::difference_type num = 0;

        for(; first != last; ++first)
            if(*first == value)
                ++num;

        return num;
    }

    template<class InputIterator, class UnaryPredicate>
    inline typename _iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, UnaryPredicate pred)
    {
        typename _iterator_traits<InputIterator>::difference_type num = 0;

        for(; first != last; ++first)
            if(UnaryPredicate(*first))
                ++num;

        return num;
    }

    template<class InputIterator, class T>
    inline InputIterator find(InputIterator first, InputIterator last, const T& value)
    {
        for(; first != last; ++first)
            if(*first == value)
                return first;

        return last;
    }
    
    template<class InputIterator, class UnaryPredicate>
    inline InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred)
    {
        for(; first != last; ++first)
            if(pred(*first))
                return first;

        return last;
    }

    template<class ForwardIterator1, class ForwardIterator2, class Distance1, class Distance2>
    inline ForwardIterator2 __search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Distance1 *, Distance2 *)
    {
        Distance1 len1 = distance(first1, last1);
        Distance2 len2 = distance(first2, last2);

        if(len1 < len2)
            return last1;
        else
        {
            ForwardIterator1 curr1 = first1;
            ForwardIterator2 curr2 = first2;
            while(curr2 != last2)
            {
                if(*curr2 == *curr1)
                {
                    ++curr1;
                    ++curr2;
                }
                else
                {
                    if(len1 == len2)
                        return last1;
                    else
                    {
                        curr1 = ++first1;
                        curr2 = first2;
                        --len1;
                    }
                }
            }

            return first1;
        }
    }

    template<class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
    {
        return __search(first1, last1, first2, last2, difference_type(first1), difference_type(first2));
    }

    template<class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 _find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag)
    {
        if(first1 == last2)
        {
            return last1;
        }
        else
        {
            ForwardIterator1 result = last1;
            while(1)
            {
                first1 = search(first1, last1, first2, last2);
                if(first1 == last1)
                    break;
                result = first1;
                ++first1;
            }
            return result;
        }
    }

    template<class RandomIterator1, class RandomIterator2>
    inline RandomIterator1 _find_end(RandomIterator1 first1, RandomIterator1 last1, RandomIterator2 first2, RandomIterator2 last2, random_iterator_tag)
    {
        return search(reverser_iterator<RandomIterator1>(first1), reverser_iterator<RandomIterator1>(last1), reverser_iterator<RandomIterator2>(first2), reverser_iterator<RandomIterator2>(last2));
    }

    template<class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
    {
        return _find_end(first1, last1, first2, last2, iterator_category(first1), iterator_category(first2));
    }

    template<class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
    {
        ForwardIterator2 curr = first2;

        for(; first1 != last1; ++first1)
            for(ForwardIterator2 iter = first2; iter != last2; ++iter)
                if(*iter == *first1)
                    return first1;

        return last1;
    }

    template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    inline ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate binary_op)
    {
        for(; first1 != last1; ++first1)
            for(ForwardIterator2 iter = first2; iter != last2; ++iter)
                if(binary_op(*iter, *first1))
                    return first1;

        return last1;
    }

    template<class InputIterator, class Function>
    inline Function for_each(InputIterator first, InputIterator last, Function func)
    {
        for(; first != last; ++first)
            func(*first);
        return func;
    }

    template<class OutputIterator, class Generator>
    void generate(OutputIterator first, OutputIterator last, Generator gen)
    {
        for(; first != last; ++first)
            *first = gen();
    }

    template<class OutputIterator, class Generator, class SizeType>
    void generate_n(OutputIterator result, SizeType n, Generator gen)
    {
        for(; n > 0; --n, ++result)
            *result = gen();
    }

    template<class InputIterator1, class InputIterator2>
    bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first2 < *first1)
                return false;
            else if(*first1 < *first2)
                ++first1;
            else
            {
                ++first1;
                ++first2;
            }
        }

        return first2 == last2;
    }

    template<class InputIterator1, class InputIterator2, class Compare>
    bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first2, *first1))
                return false;
            else if(comp(*first1, *first2))
                ++first1;
            else
            {
                ++first1;
                ++first2;
            }
        }

        return first2 == last2;
    }
    
    template<class ForwardIterator>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
    {
        if(first == last)
            return last;
        else
        {
            ForwardIterator result = first;
            ++first;
            for(; first != last; ++first)
                if(*result < *first)
                    result = first;
            return result;
        }
    }

    template<class ForwardIterator, class Compare>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
    {
        if(first == last)
            return last;
        else
        {
            ForwardIterator result = first;
            ++first;
            for(; first != last; ++first)
                if(comp(*result, *first))
                    result = first;
            return result;
        }
    }

    template<class ForwardIterator>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
    {
        if(first == last)
            return first;
        else
        {
            ForwardIterator result = first;
            ++first;
            for(; first != last; ++first)
                if(*first < *result)
                    result = first;

            return result;
        }
    }

    template<class ForwardIterator, class Compare>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
    {
        if(first == last)
            return last;
        else
        {
            ForwardIterator result = first;
            ++first;
            for(; first != last; ++first)
                if(comp(*first, *result))
                    result = first;

            return result;
        }
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first2 < *first1)
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
            }
                ++result;
        }

        return MiniStl::copy(first2, last2, MiniStl::copy(first1, last1, result));
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(comp(*first2, *first1))
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
            }
                ++result;
        }

        return MiniStl::copy(first2, last2, MiniStl::copy(first1, last1, result));
    }

    template<class BidirectionalIterator, class UnaryPredicate>
    BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate pred)
    {
        while(true)
        {
            while(true)
                if(first == last)
                    return first;
                else if(pred(*first))
                    ++first;
                else
                    break;
            --last;
            while(true)
                if(first == last)
                    return first;
                else if(!pred(*last))
                    --last;
                else
                    break;
            iter_swap(first, last);
            ++first;
        }
    }

    template<class ForwardIterator, class T>
    ForwardIterator remove_copy(ForwardIterator first, ForwardIterator last, ForwardIterator result, const T& value)
    {
        for(; first != last; ++first)
            if(*first != value)
            {
                *result = *first;
                ++result;
            }

        return result;
    }

    template<class ForwardIterator, class T>
    ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
    {
        first = find(first, last, value);
        ForwardIterator next = first;
            
        return first == last? last : remove_copy(++next, last, first);
    }

    template<class ForwardIterator, class UnaryPredicate>
    ForwardIterator remove_copy_if(ForwardIterator first, ForwardIterator last, ForwardIterator result, UnaryPredicate pred)
    {
        for(; first != last; ++first)
        {
            if(!pred(*first))
            {
                *result = *first;
                ++result;
            }
        }

        return result;
    }

    template<class ForwardIterator, class UnaryPredicate>
    ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred)
    {
        first = find_if(first, last, pred);
        ForwardIterator next = first;

        return first == last? last : remove_copy_if(++next, last, first, pred);
    }

    template<class ForwardIterator, class T>
    void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)
    {
        for(; first != last; ++first)
            if(*first == old_value)
                *first = new_value;
    }

    template<class ForwardIterator, class T, class UnaryPredicate>
    void replace_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred, const T& value)
    {
        for(; first != last; ++first)
            if(pred(*first))
                *first = value;
    }

    template<class ForwardIterator, class OutputIterator, class T>
    OutputIterator replace_copy(ForwardIterator first, ForwardIterator last, OutputIterator result, const T& old_value, const T& new_value)
    {
        for(; first != last; ++first, ++result)
            *result = *first == old_value? new_value : *first;
        return result;
    }

    template<class ForwardIterator, class OutputIterator, class T, class UnaryPredicate>
    OutputIterator replare_copy_if(ForwardIterator first, ForwardIterator last, OutputIterator result, UnaryPredicate pred, const T& new_value)
    {
        for(; first != last; ++first, ++result)
            *result = pred(*first)? new_value : *first;
        return result;
    }

    template<class BidirectionalIterator>
    void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag)
    {
        while(true)
        {
            if(first == last || first == --last)
                return;
            else
            {
                iter_swap(first, last);
                ++first;
            }
        }
    }

    template<class RandomIterator>
    void __reverse(RandomIterator first, RandomIterator last, random_iterator_tag)
    {
        while(first < last)
            iter_swap(first++, --last);
    }

    template<class BidirectionalIterator>
    void reverse(BidirectionalIterator first, BidirectionalIterator last)
    {
        __reverse(first, last, iterator_category(first));
    }

    template<class BidirectionalIterator, class OutputIterator>
    void reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result)
    {
        while(first != last)
        {
            --last;
            *result = *last;
            ++result;
        }
    }

    template<class ForwardIterator>
    void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, forward_iterator_tag)
    {
        ForwardIterator i = middle;
        while(true)
        {
            iter_swap(first, i);
            ++first;
            ++i;
            if(first == middle)
            {
                if(i == last)
                    return;
                else        // 前面先完成
                    middle = i;
            }
            else if(i == last) // 后面先完成
            {
                i = middle;
            }
        }
    }

    template<class BidirectionalIterator>
    void __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, bidirectional_iterator_tag)
    {
        reverse(first, middle);
        reverse(middle, last);
        reverse(first, last);
    }

    template<class Integer>
    Integer gcd(Integer m, Integer n)
    {
        while(n != 0)
        {
            Integer temp = m % n;
            m = n;
            n = temp;
        }

        return m;
    }

    template<class RandomIterator, class Distance, class T>
    void __rotate_cycle(RandomIterator first, RandomIterator last, RandomIterator initial, Distance n, T*)
    {
        T temp = *initial;
        RandomIterator ptr1 = initial;
        RandomIterator ptr2 = ptr1 + n;

        while(ptr2 != initial)
        {
            *ptr1 = *ptr2;
            ptr1 = ptr2;
            if(n < last - ptr2)
                ptr2 = ptr2 + n;
            else
                ptr2 = first + (n - (last - ptr2));
        }

        *ptr1 = temp;
    }

    template<class RandomIterator>
    void __rotate(RandomIterator first, RandomIterator middle, RandomIterator last, random_iterator_tag)
    {
        typename _iterator_traits<RandomIterator>::difference_type n = gcd(last - first, middle - first);

        while(n--)
            __rotate_cycle(first, last, first + n, middle - first, value_type(first));
    }

    template<class ForwardIterator>
    void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
    {
        if(first == middle || middle == last)
            return;
        else
            __rotate(first, middle, last, iterator_category(first));
    }

    template<class ForwardIterator, class OutputIterator>
    OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,ForwardIterator last, OutputIterator result)
    {
        return MiniStl::copy(first, middle, MiniStl::copy(middle, last, result));
    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size n, const T& value)
    {
        if(n <= 0)
            return first;

        first = find(first, last, value);

        while(first != last)
        {
            ForwardIterator iter = first;
            ++iter;
            Size count = n - 1;
            while(iter != last && count != 0 && value == *iter)
            {
                ++iter;
                --count;
            }
            if(count == 0)
                return first;
            else
                first = find(iter, last, value);
        }

        return last;
    }

    template<class ForwardIterator, class Size, class T, class BinaryPredicate>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size n, const T& value, BinaryPredicate binary_op)
    {
        if(n <= 0)
            return first;

        for(; first != last; ++first)
            if(binary_op(*first, value))
                break;

        while(first != last)
        {
            ForwardIterator iter = first;
            ++iter;
            Size count = n - 1;
            while(iter != last && count != 0 && binary_op(*iter, value))
            {
                ++iter;
                --count;
            }
            if(count == 0)
                return first;
            else
            {
                for(; iter != last; ++iter)
                    if(binary_op(*iter, value))
                        break;
                first = iter;
            }
        }

        return last;
    }

    template<class ForwardIterator1, class ForwardIterator2>
    ForwardIterator2 swap_range(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
    {
        for(; first1 != last1; ++first1, ++first2)
            iter_swap(first1, first2);
        return first2;
    }

    template<class ForwardIterator, class OutputIterator, class UnaryOperator>
    inline OutputIterator transform(ForwardIterator first, ForwardIterator last, OutputIterator result, UnaryOperator unary_op)
    {
        for(; first != last; ++first, ++result)
            *result = unary_op(*first);

        return result;
    }

    template<class ForwardIterator1, class ForwardIterator2, class OutputIterator, class BinaryOperator>
    inline OutputIterator transform(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, OutputIterator result, BinaryOperator binary_op)
    {
        for(; first1 != last1; ++first1, ++first2, ++result)
            *result = binary_op(*first1, *first2);
        return result;
    }

    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator _unique_copy(InputIterator first, InputIterator last, ForwardIterator result, forward_iterator_tag, T*)
    {
        *result = *first;
        ++first;
        for(; first != last; ++first)
        {
            T new_value = *first;
            if(new_value != *result)
                *++result = new_value;
        }

        return ++result;
    }

    template<class InputIterator, class OutputIterator, class T>
    inline OutputIterator _unique_copy(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag, T*)
    {
        T old_value = *first;
        ++first;
        *result = old_value;
        ++result;
        for(; first != last; ++first)
        {
            T new_value = *first;
            if(new_value != old_value)
            {
                old_value = new_value;
                *result = new_value;
                ++result;
            }
        }

        return result;
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        return _unique_copy(first, last, result, iterator_category(result), value_type(first));
    }

    template<class ForwardIterator>
    inline ForwardIterator unique(ForwardIterator first, ForwardIterator last)
    {
        first = adjacent_find(first, last);

        return unique_copy(first, last, first);
    }

    template<class InputIterator, class ForwardIterator, class BinaryPredicate, class T>
    inline ForwardIterator _unique_copy(InputIterator first, InputIterator last, ForwardIterator result, BinaryPredicate binary_op, forward_iterator_tag, T*)
    {
        *result = *first;
        ++first;
        for(; first != last; ++first)
        {
            T new_value = *first;
            
            if(!binary_op(*result, new_value))
                *++result = new_value;
        }

        return ++result;
    }

    template<class InputIterator, class OutputIterator, class BinaryPredicate, class T>
    inline OutputIterator _unique_copy(InputIterator first, InputIterator last, OutputIterator result, BinaryPredicate binary_op, output_iterator_tag, T*)
    {
        T old_value = *first;
        *result = old_value;
        ++result;
        ++first;
        for(; first != last; ++first)
        {
            T new_value = *first;
            if(!binary_op(old_value, new_value))
            {
                old_value = new_value;
                *result = new_value;
                ++result;
            }
        }

        return result;
    }

    template<class InputIterator, class OutputIterator, class UnaryPredicate>
    inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate unary_op)
    {
        if(first == last)
            return result;

        return _unique_copy(first, last, result, unary_op, iterator_category(result), value_type(first));
    }

    template<class ForwardIterator, class BinaryPredicate>
    inline ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_op)
    {
        first = adjacent_find(first, last, binary_op);

        return unique_copy(first, last, first, binary_op);
    }

}

#endif
