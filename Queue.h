#ifndef __MINISTL_QUEUE_H
#define __MINISTL_QUEUE_H
#include"Deque.h"
#include"Utility.h"

namespace MiniStl
{
    template<class T, class Container = deque<T>>
    class queue
    {
        public:
            typedef Container container_type;
            typedef typename container_type::value_type value_type;
            typedef typename container_type::reference reference;
            typedef typename container_type::const_reference const_reference;
            typedef typename container_type::size_type size_type;
        
        public:
            // constructor
            queue() : c() {}

            queue(const queue& rhs) : c(rhs.c) {}
            
            template<class InputIterator>
            queue(InputIterator first, InputIterator last) : c(first, last) {}

            bool empty() const
            {
                return c.empty();   
            }

            size_type size() const
            {
                return c.size();
            }

            void push(const value_type& value)
            {
                c.push_back(value);
            }

            void pop()
            {
                c.pop_front();
            }

            value_type front()
            {
                return c.front();
            }

            value_type back()
            {
                return c.back();
            }

            bool operator==(const queue& rhs) const
            {
                return c == rhs.c;
            }

            bool operator!=(const queue& rhs) const
            {
                return !(operator==(rhs));
            }

            void swap(queue& rhs)
            {
                using MiniStl::swap;
                swap(c, rhs.c);
            }

        private:
            container_type c;
    };

    template<class T, class Container>
    bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Container>
    bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class T, class Container>
    void swap(queue<T, Container>& lhs, queue<T, Container>& rhs)
    {
        lhs.swap(rhs);
    }
}


#endif
