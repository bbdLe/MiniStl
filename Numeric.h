#ifndef __MINISTL_NUMERIC_H
#define __MINISTL_NUMERIC_H

#include"Iterator.h"
#include"Functional.h"

namespace MiniStl
{
    template<class InputIterator, class T>
    T accumulate(InputIterator first, InputIterator last, T init)
    {
        while(first != last)
            init = init + *first++;
        return init;
    }

    template<class InputIterator, class T, class BinaryOperation>
    T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
    {
        while(first != last)
            init = binary_op(*first++, init);
        return init;
    }

    template<class InputIterator, class OutputIterator>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
    {
        if(first == last)
            return result;
        else
        {
            *result = *first;

            typedef typename _iterator_traits<InputIterator>::value_type value_type;
            value_type curr = *first;
            while(++first != last)
            {
                ++result;
                curr = *first - curr;
                *result = curr;
                curr = *first;
            }

            return ++result;
        }
    }

    template<class InputIterator, class OutputIterator, class BinaryOperator>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator binary_op)
    {
        if(first == last)
            return result;
        else
        {
            *result = *first;
            
            typedef typename _iterator_traits<InputIterator>::value_type value_type;
            value_type curr = *first;
            while(++first != last)
            {
                ++result;
                curr = binary_op(*first, curr);
                *result = curr;
                curr = *first;
            }

            return ++result;
        }
    }

    template<class InputIterator1, class InputIterator2, class T>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)
    {
        while(first1 != last1)
        {
            init = init + *first1 * *first2;
            ++first1;
            ++first2;
        }

        return init;
    }

    template<class InputIterator1, class InputIterator2, class T, class BinaryOperator1, class BinaryOperator2>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperator1 binary_op1, BinaryOperator2 binary_op2)
    {
        while(first1 != last1)
        {
            init = binary_op1(init, binary_op2(*first1, *first2));
            ++first1;
            ++first2;
        }

        return init;
    }

    template<class InputIterator, class OutputIterator>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
    {
        if(first == last)
            return result;
    
        *result = *first;
        typedef typename _iterator_traits<InputIterator>::value_type value_type;
        value_type curr = *first;
        while(++first != last)
        {
            ++result;
            curr = curr + *first;
            *result = curr;
        }

        return ++result;
    }
    
    template<class InputIterator, class OutputIterator, class BinaryOperator>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator binary_op)
    {
        if(first == last)
            return result;

        *result = *first;
        typedef typename _iterator_traits<InputIterator>::value_type value_type;
        value_type curr = *first;
        while(++first != last)
        {
            ++result;
            curr = curr + *first;
            *result = curr;
        }

        return ++result;
    }

    template<class ForwardIterator, class T>
    void itoa(ForwardIterator first, ForwardIterator last, T val)
    {
        while(first != last)
        {
            *first = val;
            ++first;
            ++val;
        }
    }

    template<class T, class Integer>
    T power(T x, Integer n)
    {
        return power(x, n, multiplies<T>());
    }

    template<class T, class Integer, class BinaryOperator>
    T power(T x, Integer n, BinaryOperator binary_op)
    {
        if(n == 0)
            return identity_element(binary_op);
        else
        {
            while((n & 1) == 0)
            {
                n >>= 1;
                x = binary_op(x, x);
            }

            T result = x;
            n >>= 1;
            while(n != 0)
            {
                x = binary_op(x, x);
                if((n & 1) != 0)
                    result = binary_op(result, x);
                n >>= 1;
            }

            return result;
        }
    }
}


#endif
