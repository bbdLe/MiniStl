#ifndef __MINISTL_DEQUE_H
#define __MINISTL_DEQUE_H
#include"Uninitialized.h"
#include"Allocator.h"
#include"Iterator.h"
#include<cstddef>
#include<iostream>
#include<cassert>
#include<utility>

namespace MiniStl
{
    inline size_t _deque_buffer_size(size_t n, size_t size)
    {
        return n != 0? n : size > 512? size : static_cast<size_t>(512) / size;
    }

    template<class T, class Pointer, class Reference, size_t BuffSize>
    struct _deque_iterator
    {
        typedef random_iterator_tag iterator_category;
        typedef T value_type;
        typedef Pointer pointer;
        //typedef const Pointer const_pointer;
        typedef Reference reference;
        //typedef const Reference const_reference;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T** map_pointer;
        typedef _deque_iterator<T, T*, T&, BuffSize> iterator;
        typedef _deque_iterator<T, const T*, const T&, BuffSize> const_iterator;
        typedef _deque_iterator<T, Pointer, Reference, BuffSize> self;
        // data
        T* curr;
        T* first;
        T* last;
        map_pointer node;

        //constructor
        _deque_iterator() : curr(NULL), first(NULL), last(NULL), node(NULL) {}

        _deque_iterator(const iterator& x) : curr(x.curr), first(x.first), last(x.last), node(x.node) {}

        // operator
        self& operator=(const self &other)
        {
            curr = other.curr;
            first = other.first;
            last = other.last;
            node = other.node;
            return *this;
        }

        self& operator++()
        {
            if(curr != last)
            {
                ++curr;
            }
            else
            {
                set_node(node + 1);
                curr = first;
            }
            return *this;
        }

        self operator++(int)
        {
            iterator temp = *this;
            operator++();
            return temp;
        }

        self& operator--()
        {
            if(curr != first)
            {
                --curr;
            }
            else
            {
                set_node(node - 1);
                curr = last;
                --curr;
            }
            return *this;
        }

        self operator--(int)
        {
            iterator temp = *this;
            operator--();
            return temp;
        }

        value_type& operator*()
        {
            return *curr;
        }

        pointer operator->()
        {
            return &(operator*());
        }

        self& operator+=(difference_type n)
        {
            difference_type offset = (curr - first) + n;
            if(offset >= 0 && offset < static_cast<difference_type>(buffer_size()))
            {
                curr += n;
            }
            else
            {
                difference_type node_offset = offset > 0? offset / static_cast<difference_type>(buffer_size())
                                                        : ((offset + 1) / static_cast<difference_type>(buffer_size()) - 1);
                set_node(node + node_offset);
                curr = first + offset - static_cast<difference_type>(buffer_size()) * node_offset;
            }
            return *this;
        }

        self operator+(difference_type n) const
        {
            iterator temp = *this;
            temp += n;
            return temp;
        }

        self& operator-=(difference_type n)
        {
            *this += -n;
            return *this;
        }

        self operator-(difference_type n) const
        {
            iterator temp = *this;
            temp -= n;
            return temp;
        }

        difference_type operator-(const self& rhs)
        {
            return (node - rhs.node - 1) * buffer_size() + (curr - first) + (rhs.last - rhs.curr);
        }

        bool operator==(const self& rhs) const
        {
            return curr == rhs.curr;
        }

        bool operator!=(const self& rhs) const
        {
            return !(operator==(rhs));
        }

        bool operator<(const self& rhs) const
        {
            return node == rhs.node? curr < rhs.curr : node < rhs.node;
        }

        // inside
        void set_node(map_pointer newNode)
        {
            node = newNode;
            first = *newNode;
            last = first + static_cast<difference_type>(buffer_size());
        }

        size_type buffer_size()
        {
            return _deque_buffer_size(BuffSize, sizeof(T));
        }
    };

    template<class T, class Alloc = myAlloc, size_t BufferSize = 0>
    class deque
    {
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef T& reference;
            typedef ptrdiff_t difference_type;
            typedef size_t size_type;
            typedef const T* const_pointer;
            typedef const T& const_reference;
            typedef _deque_iterator<T, T*, T&, BufferSize> iterator;
            typedef _deque_iterator<T, const T*, const T&, BufferSize> const_iterator;
            typedef T** map_pointer;

        private:
            typedef simple_alloc<value_type, Alloc> data_allocator;
            typedef simple_alloc<pointer, Alloc> map_allocator;
        
        private:
            //data
            iterator start;
            iterator finish;
            map_pointer map;
            size_type map_size;
        
        public:
            // constructor
            deque() : start(), finish(), map(NULL), map_size(0)
            {
                create_map_and_nodes(0);
            }

            // copy constructor
            deque(const deque& rhs)
            {
                create_map_and_nodes(rhs.size());
                uninitialized_copy(rhs.begin(), rhs.end(), start);
            }

            // constructor
            deque(size_type n, const value_type& value)
            {
                fill_initialize(n, value);
            }

            deque(size_type n)
            {
                fill_initialize(n, value_type());
            }

            template<class InputIterator>
            deque(InputIterator first, InputIterator last)
            {
                range_initialize(first, last, iterator_category(first));
            }

            ~deque()
            {
                destory(start, finish);
                destroy_map_and_nodes();
            }

            // iterator
            iterator begin()
            {
                return start;
            }

            const_iterator begin() const
            {
                return start;
            }

            const_iterator cbegin() const
            {
                return start;
            }

            iterator end()
            {
                return finish;
            }

            const_iterator end() const
            {
                return finish;
            }

            const_iterator cend() const
            {
                return finish;
            }

            bool operator==(const deque& rhs) const
            {
                if(size() != rhs.size())
                    return false;
                else
                {
                    auto first1 = begin();
                    auto last1 = end();
                    auto first2 = rhs.begin();
                    auto last2 = rhs.end();

                    for(; first1 != last1 && first2 != last2 && *first1 == *first2; ++first1, ++first2)
                        ;

                    return (first1 == last1) && (first2 == last2);
                }
            }

            bool operator!=(const deque& rhs)
            {
                return !(operator==(rhs));
            }

            // capacity
            size_type size() const
            {
                return static_cast<size_type>(end() - begin());
            }

            void resize(size_type n)
            {
                resize(n, value_type());
            }

            void resize(size_type n, const value_type& value)
            {
                if(n == size())
                    return;

                if(n < size())
                {
                    size_type diff = size() - n;
                    erase(finish - static_cast<difference_type>(diff), finish);
                }
                else
                {
                    size_type diff = n - size();
                    insert(finish, diff, value);
                }
            }

            void shrink_to_fit()
            {
                deque nDeque(start, finish);
                swap(nDeque);
            }
            
            bool empty() const
            {
                return start == finish;
            }

            // element access
            reference front()
            {
                return *start;
            }

            reference back()
            {
                return *(finish - 1);
            }

            reference operator[](difference_type n)
            {
                return *(start + n);
            }

            // modifiers
            void push_back(const value_type& value)
            {
                // 存在一个以上的空间
                if(finish.curr != finish.last - 1)
                {
                    construct(finish.curr, value);
                    ++finish;
                }
                else
                {
                    push_back_aux(value);
                }
            }

            void push_front(const value_type& value)
            {
                if(start.curr != start.first)
                {
                    construct(start.curr - 1, value);
                    --start;
                }
                else
                {
                    push_front_aux(value);
                }
            }

            void pop_front()
            {
                if(start.curr != start.last - 1)
                {
                    destory(&*start);
                    ++start;
                }
                else
                {
                    destory(&*start);
                    deallocate_node(*(start.node));
                    start.set_node(start.node + 1);
                    start.curr = start.first;
                }
            }

            void pop_back()
            {
                if(finish.curr != finish.first)
                {
                    destory(&*finish);
                    --finish;
                }
                else
                {
                    deallocate_node(*(finish.node));
                    finish.set_node(finish.node - 1);
                    finish.curr = finish.last - 1;
                    destory(&*finish);
                }
            }

            iterator insert(iterator position, const value_type& value)
            {
                if(position == start)
                {
                    push_front(value);
                    return start;
                }
                else if(position == finish)
                {
                    push_back(value);
                    return finish;
                }
                else
                {
                    difference_type numElemsBefore = position - start;
                    difference_type totalSize = size();
                    iterator newPosition;
                    // 前面的对象比较少
                    if(numElemsBefore < (totalSize >> 1))
                    {
                        push_front(front());
                        newPosition = start + numElemsBefore + 1;
                        iterator result = start;
                        ++result;
                        iterator first = result;
                        ++first;
                        copy(first, newPosition, result);
                        construct((newPosition - 1).curr, value);
                    }
                    // 后面的对象比较少
                    else
                    {
                        push_back(back());
                        newPosition = start + numElemsBefore;
                        iterator result = finish;
                        --result;
                        iterator last = result;
                        --last;
                        assert(result - 1 == last);
                        for(auto beg = newPosition; beg != last; ++beg)
                            std::cout << *beg << std::endl;
                        copy_backward(newPosition, last, result);
                        construct(newPosition.curr, value);
                    }
                    return newPosition;
                } 
            }

            iterator erase(iterator position)
            {
                difference_type elementsBefore = position - start;
                difference_type totalSize = size();
                if(elementsBefore < (totalSize > 1))
                {
                    copy_backward(start, position, position + 1);
                    pop_front();
                    return start + elementsBefore - 1;
                }
                else
                {
                    copy(position + 1, finish, position);
                    pop_back();
                    return start + elementsBefore;
                }
            }

            void erase(iterator first, iterator last)
            {
                difference_type len = last - first;
                difference_type elementsBefore = first - start;
                difference_type totalSize = size();
                if(elementsBefore < (totalSize > 1))
                {
                    iterator newStart = start + len;
                    copy_backward(start, first, last);
                    destory(start, newStart);
                    for(map_pointer ptr = start.node; ptr != newStart.node; ++ptr)
                        deallocate_node(*ptr);
                    start = newStart;
                }
                else
                {
                    iterator newFinish = finish - len;
                    copy(last, finish, first);
                    destory(newFinish, finish);
                    for(map_pointer ptr = finish.node; ptr != newFinish.node; --ptr)
                        deallocate_node(*ptr);
                    finish = newFinish;
                }
            }
            
            template<class InputIterator>
            void insert(iterator position, InputIterator first, InputIterator last)
            {
                if(first != last)
                    insert_range_aux(position, first, last, iterator_category(first));
            }

            void insert(iterator position, size_type n, const value_type& value)
            {
                if(position == start)
                {
                    iterator newStart = reserve_elements_at_front(n);
                    uninitialized_fill_n(newStart, n, value);
                    start = newStart;
                }
                else if(position == finish)
                {
                    iterator newFinish = reserve_elements_at_back(n);
                    uninitialized_fill_n(finish, n, value);
                    finish = newFinish;
                }
                else
                {
                    insert_aux(position, n, value);
                }
            }

            void swap(deque& rhs)
            {
                using MiniStl::swap;
                swap(start, rhs.start);
                swap(finish, rhs.finish);
                swap(map, rhs.map);
            }

            void clear()
            {
                destory(start, finish);
                for(map_pointer ptr = finish.node; ptr != start.node; --ptr)
                {
                    data_allocator::deallocate(*ptr);
                }
                finish = start;
            }

        private:
            void create_map_and_nodes(size_type num_elements)
            {
                // 整除会多出一个node
                size_type num_nodes = num_elements / buffer_size() + 1;
                map_size = initial_map_size() > num_nodes ? initial_map_size() : num_nodes + 2;
                map = map_allocator::allocate(map_size);
                
                map_pointer nstart = map + (map_size - num_nodes) / 2;
                map_pointer nend = nstart + num_nodes - 1;
                for(map_pointer curr = nstart; curr <= nend; ++curr)
                {
                    *curr = allocate_node();
                }
                start.set_node(nstart);
                finish.set_node(nend);
                start.curr = start.first;
                finish.curr = finish.first + num_elements % buffer_size();
            }

            void destroy_map_and_nodes()
            {
                for(map_pointer curr = start.node; curr <= finish.node; ++curr)
                {
                    deallocate_node(*curr);
                }
                map_allocator::deallocate(map); 
            }

            static size_type buffer_size()
            {
                return _deque_buffer_size(BufferSize, sizeof(T));           
            }
            
            static size_type initial_map_size()
            {
                return 8;
            }

            static pointer allocate_node()
            {
                return data_allocator::allocate(buffer_size());
            }

            static void deallocate_node(pointer ptr)
            {
                data_allocator::deallocate(ptr);
            }

            void fill_initialize(size_t n, const value_type &value)
            {
                create_map_and_nodes(n);
                map_pointer curr;
                uninitialized_fill_n(start, n, value);
                /*for(curr = start.node; curr < finish.node; ++curr)
                    uninitialized_fill(*curr, *curr + buffer_size(), value);
                uninitialized_fill(finish.first, finish.curr, value);*/
            }
            
            // for input_iterator_tag
            template<class InputIterator>
            void range_initialize(InputIterator first, InputIterator end, input_iterator_tag)
            {
                create_map_and_nodes(0);
                for(; first != end; ++first)
                    push_back(*first);
            }

            // forward_iterator_tag     
            template<class ForwardIterator>
            void range_initialize(ForwardIterator first, ForwardIterator end, forward_iterator_tag)
            {
                typedef typename _iterator_traits<ForwardIterator>::difference_type Distance;
                
                // Fixme
                Distance n = distance(first, end);
                create_map_and_nodes(n);
                uninitialized_copy(first, end, start);
            }

            void push_back_aux(const value_type& value)
            {
                reserve_map_at_back();
                *(finish.node + 1) = allocate_node();
                construct(finish.curr, value);
                finish.set_node(finish.node + 1);
                finish.curr = finish.first;
            }

            void push_front_aux(const value_type& value)
            {
                reserve_map_at_front();
                *(start.node - 1) = allocate_node();
                start.set_node(start.node - 1);
                start.curr = start.last - 1;
                construct(start.curr, value);
            }

            template <class InputIterator>
            void insert_range_aux(iterator position, InputIterator first, InputIterator last, input_iterator_tag)
            {
                for(; first != last; ++first)
                    position = insert(position, *first);
            }

            template <class ForwardIterator>
            void insert_range_aux(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag)
            {
                typedef typename _iterator_traits<ForwardIterator>::difference_type Distance;
                
                Distance len = distance(first, last);
                if(position == start)
                {
                    iterator newStart = reserve_elements_at_front(len);
                    uninitialized_copy(first, last, newStart);
                    start = newStart;
                }
                else if(position == finish)
                {
                    iterator newFinish = reserve_elements_at_back(len);
                    uninitialized_copy(first, last, finish);
                    finish = newFinish;
                }
                else
                {
                    insert_range_aux(position, first, last, len);
                }
            }

            template<class ForwardIterator>
            void insert_range_aux(iterator position, ForwardIterator first, ForwardIterator last, typename _iterator_traits<ForwardIterator>::difference_type len)
            {
                difference_type numElemsBefore = position - start;
                difference_type totalSize = size();
                // 向前移动元素
                if(numElemsBefore < (totalSize > 1))
                {
                    iterator newStart = reserve_elements_at_front(len);
                    iterator oldStart = start;
                    iterator newPosition = oldStart + numElemsBefore;
                    start = newStart;
                    if(len <= numElemsBefore)
                    {
                        uninitialized_copy(oldStart, oldStart + len, newStart);
                        copy(oldStart + len, newPosition, oldStart);
                        copy(first, last, newPosition - len);
                    }
                    else
                    {
                        ForwardIterator mid = first;
                        advance(mid, len - numElemsBefore);
                        _uninitialized_copy_copy(oldStart, newPosition, first, mid, newStart);
                        copy(mid, last, oldStart);
                    }
                }
                else
                {
                    iterator newFinish = reserve_elements_at_back(len);
                    iterator oldFinish = finish;
                    iterator newPosition = start + numElemsBefore;
                    finish = newFinish;
                    difference_type numElemsAfter = oldFinish - newPosition;
                    if(len <= numElemsAfter)
                    {
                        uninitialized_copy(oldFinish - len, oldFinish, oldFinish);
                        copy_backward(newPosition, oldFinish - len, oldFinish);
                        copy(first, last, newPosition);
                    }
                    else
                    {
                        ForwardIterator mid = first;
                        advance(mid, numElemsAfter);
                        _uninitialized_copy_copy(newPosition, oldFinish, mid, last, oldFinish);
                        copy(first, mid, newPosition);
                    }
                }
            }

            void insert_aux(iterator position, size_type n, const value_type& value)
            {
                difference_type elementsBefore = position - start;
                difference_type totalSize = size();
                if(elementsBefore < (totalSize >> 1))
                {
                    iterator newStart = reserve_elements_at_front(n);
                    iterator oldStart = start;
                    start = newStart;
                    iterator newPosition = oldStart + elementsBefore;
                    if(n < elementsBefore)
                    {
                        uninitialized_copy(oldStart, oldStart + static_cast<difference_type>(n), newStart);
                        copy(oldStart + static_cast<difference_type>(n), newPosition, oldStart);
                        fill_n(newPosition - n, n, value);
                    }
                    else
                    {
                        uninitialized_copy(oldStart, newPosition, newStart);
                        uninitialized_fill_n(newStart + static_cast<difference_type>(elementsBefore), n - elementsBefore, value);
                        fill_n(oldStart, elementsBefore, value);
                    }
                }
                else
                {
                    difference_type elementsAfter = finish - position;
                    iterator newFinish = reserve_elements_at_back(n);
                    iterator oldFinish = finish;
                    finish = newFinish;
                    iterator newPosition = start + elementsBefore;
                    if(n < elementsAfter)
                    {
                        uninitialized_copy(oldFinish - n, oldFinish, oldFinish);
                        copy_backward(newPosition, oldFinish - n, oldFinish);
                        fill_n(newPosition, n, value);
                    }
                    else
                    {
                        uninitialized_copy(newPosition, oldFinish, oldFinish + n - elementsAfter);
                        uninitialized_fill_n(oldFinish, n - elementsAfter, value);
                        fill_n(newPosition, elementsAfter, value);
                    }
                }
            }

            iterator reserve_elements_at_front(difference_type len)
            {
                difference_type freeSpace = start.curr - start.first;
                if(len > freeSpace)
                {
                    size_type needNumNodes = (static_cast<size_type>(len - freeSpace) + buffer_size() - 1) / buffer_size();
                    reserve_map_at_front(needNumNodes);
                    for(map_pointer ptr = start.node - needNumNodes; ptr != start.node; ++ptr)
                    {
                        *ptr = allocate_node();
                    }
                }
                return start - len;
            }

            iterator reserve_elements_at_back(difference_type len)
            {
                difference_type freeSpace = finish.last - finish.curr;
                if(len > freeSpace)
                {
                    size_type needNumNodes = (static_cast<size_type>(len - freeSpace) + buffer_size() - 1) / buffer_size();
                    reserve_map_at_back(needNumNodes);
                    for(map_pointer ptr = finish.node + needNumNodes; ptr != finish.node; --ptr)
                    {
                        *ptr = allocate_node();
                    }
                }
                return finish + len;
            }

            void reserve_map_at_front(size_type nodes_to_add = 1)
            {
                if(start.node - map < nodes_to_add)
                    reallocate_map(nodes_to_add, true);
            }

            void reserve_map_at_back(size_type nodes_to_add = 1)
            {
                if(map_size - (finish.node - map) - 1 < nodes_to_add)
                    reallocate_map(nodes_to_add, false);
            }

            void reallocate_map(size_type nodes_to_add, bool is_front)
            {
                size_type oldNumNodes = finish.node - start.node + 1;
                size_type newNumNodes = oldNumNodes + nodes_to_add;

                map_pointer newStart;
                if(map_size > newNumNodes * 2)
                {
                    newStart = map + (map_size - newNumNodes) / 2 + (is_front? nodes_to_add : 0);
                    if(newStart < start.node)
                        copy(start.node, finish.node, newStart);
                    else
                        copy_backward(start.node, finish.node, newStart);
                }
                else
                {
                    size_type newMapSize = map_size + (map_size > newNumNodes? map_size : newNumNodes) + 2;
                    map_pointer newMap = map_allocator::allocate(newMapSize);
                    newStart = newMap + (newMapSize - newNumNodes) / 2 + (is_front? nodes_to_add : 0);
                    copy(start.node, finish.node, newStart);
                    map_allocator::deallocate(map);
                    map = newMap;
                    map_size = newMapSize;
                }
                start.node = newStart;
                finish.node = start.node + newNumNodes - 1;
            }

    };

    template<class T, class Alloc, size_t BufferSize>
    bool operator==(const deque<T, Alloc, BufferSize>& lhs, const deque<T, Alloc, BufferSize>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Alloc, size_t BufferSize>
    bool operator!=(const deque<T, Alloc, BufferSize>& lhs, const deque<T, Alloc, BufferSize>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class T, class Alloc, size_t BufferSize>
    void swap(deque<T, Alloc, BufferSize>& lhs, deque<T, Alloc, BufferSize>& rhs)
    {
        lhs.swap(rhs);
    }
}
#endif
