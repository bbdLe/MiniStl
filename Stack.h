#ifndef __MINISTL_STACK_H
#define __MINISTL_STACK_H
#include"Deque.h"

namespace MiniStl
{
    template<class T, class Container = deque<T>>
    class stack
    {
        public:
            typedef Container container_type;
            typedef typename container_type::value_type value_type;
            typedef typename container_type::reference reference;
            typedef typename container_type::const_reference const_reference;
            typedef typename container_type::size_type size_type;

        public:
            // constructor
            stack() : c() {}
            
            stack(const stack& rhs) : c(rhs.c) {}
            
            template<class InputIterator>
            stack(InputIterator first, InputIterator last) : c(first, last) {}

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
                c.pop_back();
            }

            const_reference top() const
            {
                return c.back();
            }

            reference top()
            {
                return c.back();
            }

            bool operator==(const stack& rhs) const
            {
                return c == rhs.c;
            }

            bool operator!=(const stack& rhs) const
            {
                return !(operator==(rhs));
            }

        private:
            container_type c;
    };
    
    template<class T, class Container>
    bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Container>
    bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return !(lhs == rhs);
    }
}


#endif
