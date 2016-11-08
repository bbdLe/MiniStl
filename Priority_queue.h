#ifndef __MINISTL_PRIORITY_QUEUE_H
#define __MINISTL_PRIORITY_QUEUE_H
#include"Vector.h"
#include"Algobase.h"
#include"Functional.h"

namespace MiniStl
{
    // By default, it's a max heap:) 
    template<class T, class Compare = MiniStl::less<T>, class Sequence = vector<T>>
    class priority_queue
    {
        public:
            typedef typename Sequence::value_type value_type;
            typedef typename Sequence::reference reference;
            typedef typename Sequence::const_reference const_reference;
            typedef typename Sequence::size_type size_type;
    
        private:
            Sequence c;
            Compare comp;

        public:
            priority_queue() : c(), comp() {}
        
            priority_queue(const Compare& x) : c(), comp(x) {}
        
            priority_queue(const priority_queue& rhs) : c(rhs.c), comp(rhs.x) {}

            template<class InputIterator>
            priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x)
            {
                make_heap(c.begin(), c.end(), comp);
            }

            template<class InputIterator>
            priority_queue(InputIterator first, InputIterator last) : c(first, last), comp()
            {
                make_heap(c.begin(), c.end(), comp);
            }

        public:
            const_reference top() const
            {
                return c.front();
            }

            void push(const value_type& value)
            {
                c.push_back(value);
                push_heap(c.begin(), c.end(), comp);
            }

            void pop()
            {
                pop_heap(c.begin(), c.end(), comp);
                c.pop_back();
            }

            size_type size() const
            {
                return c.size();
            }

            bool empty() const
            {
                return c.empty();
            }

    };
}
#endif
