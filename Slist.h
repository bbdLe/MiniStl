#ifndef __MINISTL_SLIST_H
#define __MINISTL_SLIST_H

#include<cstddef>
#include"Iterator.h"
#include"Alloc.h"
#include"Allocator.h"

namespace MiniStl
{
    // 一级结构
    struct _slist_node_base
    {
        _slist_node_base *next;

        _slist_node_base() : next(NULL) {}
    };

    // 二级结构
    template<class T>
    struct _slist_node : public _slist_node_base
    {
        T data;
    };

    // 一级迭代器
    struct _slist_base_iterator
    {
        typedef size_t size_type;
        typedef ptrdiff_t difference;
        typedef forward_iterator_tag iterator_category;

        // data
        _slist_node_base* node;

        _slist_base_iterator() : node(NULL) {}

        _slist_base_iterator(_slist_node_base* ptr) : node(ptr) {}

        _slist_base_iterator(const _slist_base_iterator& rhs) : node(rhs.node) {}

        void incr()
        {
            node = node->next;
        }

        bool operator==(const _slist_base_iterator& rhs) const
        {
            return node == rhs.node;
        }

        bool operator!=(const _slist_base_iterator& rhs) const
        {
            return node != rhs.node;
        }
    };

    template<class T, class Pointer, class Reference>
    struct slist_iterator : public _slist_base_iterator
    {
        typedef T value_type;
        typedef Reference reference;
        typedef Pointer pointer;
        typedef slist_iterator<T, const T*, const T&> const_iterator;
        typedef slist_iterator<T, T*, T&> iterator;
        typedef slist_iterator<T, Pointer, Reference> self;
        typedef _slist_node<T> slist_node;

        slist_iterator() {}

        slist_iterator(slist_node* ptr) : _slist_base_iterator(ptr) {}

        slist_iterator(const slist_iterator &rhs) : _slist_base_iterator(rhs) {}

        self& operator++()
        {
            incr();
            return *this;
        }

        self operator++(int)
        {
            self temp = *this;
            operator++();
            return temp;
        }

        reference operator*()
        {
            return static_cast<slist_node*>(node)->data;
        }

        reference operator->()
        {
            return &(operator*());
        }
    };
    
    inline size_t _slist_size(_slist_node_base* first)
    {
        size_t n = 0;
        for(; first != NULL; first = first->next, ++n)
            ;
        return n;
    }

    inline _slist_node_base* _slist_make_link(_slist_node_base* prev_node, _slist_node_base* new_node)
    {
        new_node->next = prev_node->next;
        prev_node->next = new_node;
        return new_node;
    }

    // 多个函数会用到这个函数
    inline _slist_node_base* _slist_previous(_slist_node_base* header, _slist_node_base* target)
    {
        while(header != NULL && header->next != target)
            header = header->next;
        return header;
    }

    // 寻找前继
    inline const _slist_node_base* _slist_previous(const _slist_node_base* header, const _slist_node_base* target)
    {
        while(header != NULL && header->next != target)
            header = header->next;
        return header;
    }

    // 反转链表辅助函数
    inline _slist_node_base* _slist_reverse(_slist_node_base* node)
    {
        _slist_node_base* result = node;
        result->next = NULL;
        _slist_node_base* next_node = node->next;
        while(next_node != NULL)
        {
            _slist_node_base* temp = next_node->next;
            next_node->next = result;
            result = next_node;
            next_node = temp;
        }

        return result;
    }

    // 将[before_first + 1, before_last]插入到position后边
    inline void _slist_splice_after(_slist_node_base* position, _slist_node_base* before_first, _slist_node_base* before_last)
    {
        _slist_node_base* first = before_first->next;
        _slist_node_base* last = before_last->next;
        before_first->next = last;
        before_last->next = position->next;
        position->next = first;
    }

    template<class T, class Alloc = myAlloc>
    class slist
    {
        public:
            typedef T value_type; // (1)
            typedef T* pointer;   // (2)
            typedef T& reference; // (3)
            typedef const T* const_pointer;
            typedef const T& const_reference;
            typedef ptrdiff_t difference_type;  // (4)
            typedef size_t size_type;   // (5)
            typedef slist_iterator<T, T*, T&> iterator;
            typedef slist_iterator<T, const T*, const T&> const_iterator;

        private:
            typedef _slist_node_base slist_base_node;
            typedef _slist_node<T> slist_node;
            typedef simple_alloc<slist_node, Alloc> slist_node_allocator;

            _slist_node_base head;

        public:
            // constructor
            slist(){}
            
            slist(size_type n)
            {
                _slist_uninitialized_fill_n(n, T());
            }

            slist(size_type n, const value_type& value)
            {
                _slist_uninitialized_fill_n(n, value);
            }

            template<class InputIterator>
            slist(InputIterator first, InputIterator last)
            {
                slist_base_node* curr = &head;
                for(; first != last; curr = curr->next, ++first)
                {
                    slist_node* new_node = create_node(*first);
                    _slist_make_link(curr, static_cast<slist_base_node*>(new_node));
                }
            }

            // destructor
            ~slist()
            {
                destory_and_deallocate();
            }

            // capacity
            size_type size() const
            {
                return _slist_size(head.next);
            }

            bool empty() const
            {
                return head.next == NULL;
            }

            // element access
            reference front()
            {
                return *begin();
            }

            const_reference front() const
            {
                return *begin();
            }

            reference back()
            {
                slist_base_node* lastElement = back_aux();
                return static_cast<slist_node*>(lastElement)->data;
            }

            const_reference back() const
            {
                slist_base_node* lastElement = back_aux();
                return static_cast<slist_node*>(lastElement)->data;
            }

            // iterator
            iterator begin()
            {
                return iterator(static_cast<slist_node*>(head.next));
            }

            const_iterator begin() const
            {
                return const_iterator(static_cast<slist_node*>(head.next));
            }

            const_iterator cbegin() const
            {
                return const_iterator(static_cast<slist_node*>(head.next));
            }

            iterator end()
            {
                return iterator(NULL);
            }

            const_iterator end() const
            {
                return const_iterator(NULL);
            }

            const_iterator cend() const
            {
                return const_iterator(NULL);
            }

            // modifier
            void resize(size_type n)
            {
                resize(n, value_type());
            }

            void resize(size_type n, const value_type& value)
            {
                slist_base_node* prev = &head;
                slist_base_node* curr = head.next;
                size_type count = 0;
                for(; curr && count != n; ++count, prev = curr, curr = curr->next)
                    ;
                if(count == n)
                    return;
                else if(curr)
                {
                    erase_after(iterator(static_cast<slist_node*>(prev)), end());
                }
                else
                {
                    insert_after(iterator(static_cast<slist_node*>(prev)), n - count, value);
                }
            }


            void clear()
            {
                destory_and_deallocate();
            }


            void push_front(const value_type& value)
            {
                slist_node* newNode = create_node(value);
                _slist_make_link(&head, static_cast<slist_base_node*>(newNode));
            }

            void pop_front()
            {
                slist_base_node* temp = head.next;
                head.next = temp->next;
                destroy_node(temp);
            }

            iterator insert(iterator position, const value_type& value)
            {
                slist_base_node *prev = _slist_previous(&head, position.node);
                slist_node* new_node = create_node(value);
                _slist_make_link(prev, static_cast<slist_base_node*>(new_node));
                return iterator(new_node);
            }

            // 比单独调用insert效率高
            template <class InputIterator>
            void insert(iterator position, InputIterator first, InputIterator last)
            {
                if(first != last)
                {
                    slist_base_node *prev = _slist_previous(&head, position.node);
                    slist_node* first_node = create_node(*first);
                    slist_node* last_node = first_node;
                    ++first;
                    for(; first != last; ++first)
                    {
                        slist_node* curr_node = create_node(*first);
                        last_node->next = static_cast<slist_base_node*>(curr_node);
                        last_node = curr_node;
                    }
                    prev->next = first_node;
                    last_node->next = position.node;
                }
            }

            iterator insert_after(iterator position, const value_type& value)
            {
                slist_node* new_node = create_node(value);
                _slist_make_link(position.node, static_cast<slist_base_node*>(new_node));
                return iterator(new_node);
            }

            template<class InputIterator>
            iterator insert_after(iterator position, InputIterator first, InputIterator last)
            {
                if(first != last)
                {
                    slist_node* first_node = create_node(*first);
                    slist_node* last_node = first_node;
                    ++first;
                    for(; first != last; ++first)
                    {
                        slist_node* new_node = create_node(*first);
                        last_node->next = static_cast<slist_base_node*>(new_node);
                        last_node = new_node;
                    }

                    last_node->next = position.node->next;
                    position.node->next = static_cast<slist_base_node*>(first_node);
                    return iterator(first_node);
                }
            }

            iterator insert_after(iterator position, size_type n, const value_type& value)
            {
                if(n == 0)
                    return position;
                slist_node* first_node = create_node(value);
                slist_node* last_node = first_node;
                --n;
                for(; n > 0; --n)
                {
                    slist_node* new_node = create_node(value);
                    last_node->next = new_node;
                    last_node = new_node;
                }
                last_node->next = position.node->next;
                position.node->next = first_node;
                return iterator(first_node);
            }

            iterator erase(iterator position)
            {
                slist_base_node *prev = _slist_previous(&head, position.node);
                prev->next = prev->next->next;
                destroy_node(position.node);
                return iterator(static_cast<slist_node*>(prev->next));
            }

            // 效率较高
            void erase(iterator first, iterator last)
            {
                slist_base_node *prev = _slist_previous(&head, first.node);
                
                prev->next = last.node;
                for(; first != last; ++first)
                    destory(first.node);
            }

            iterator erase_after(iterator position)
            {
                slist_base_node *temp = position.node->next;
                position.node->next = temp->next;
                destroy_node(temp);
                return iterator(static_cast<slist_node*>(position.node->next));
            }

            iterator erase_after(iterator position_before, iterator last)
            {
                slist_base_node *temp = position_before.node->next;
                position_before.node->next = last.node;
                
                while(temp != last.node)
                {
                    slist_base_node *next_node = temp->next;
                    destroy_node(temp);
                    temp = next_node;
                }
                return last;
            }

            void remove(const value_type& value)
            {
                slist_base_node *prev = &head;
                slist_base_node *curr = head.next;

                while(curr != NULL)
                {
                    if(*curr == value)
                    {
                        prev->next = curr->next;
                        destroy_node(curr);
                        curr = prev->next;
                    }
                    else
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                }
            }

            template<class Predicate>
            void remove(Predicate pred)
            {
                slist_base_node* prev_node = &head;
                slist_base_node* curr_node = head.next;

                while(curr_node)
                {
                    if(pred(static_cast<slist_node*>(curr_node)->data))
                    {
                        prev_node->next = curr_node->next;
                        destroy_node(curr_node);
                        curr_node = prev_node->next;
                    }
                    else
                    {
                        prev_node = curr_node;
                        curr_node = curr_node->next;
                    }
                }
            }

            void reverse()
            {
                if(!empty())
                {
                    head.next = _slist_reverse(head.next);
                }
            }

            void unique()
            {
                if(head.next == NULL || head.next->next == NULL)
                    return;
                slist_base_node *prev = head.next;
                slist_base_node *curr = prev->next;
                while(curr != NULL)
                {
                    if(static_cast<slist_node*>(curr)->data == static_cast<slist_node*>(prev)->data)
                    {
                        erase_after(iterator(static_cast<slist_node*>(prev)));
                        curr = prev->next;
                    }
                    else
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                }
            }

            void sort()
            {
                if(head.next == NULL || head.next->next == NULL)
                    return;

                slist array[64];
                slist carry;
                int i = 0;
                int fill = 0;
                while(!empty())
                {
                    _slist_splice_after(&carry.head, &head, head.next);
                    while(i < fill && !array[i].empty())
                    {
                        array[i].merge(carry);
                        carry.swap(array[i++]);
                    }
                    carry.swap(array[i]);
                    if(i == fill)
                        ++fill;
                }

                for(int count = 0; count < fill; ++count)
                {
                    carry.merge(array[count]);
                }
                carry.swap(*this);
            }

            void merge(slist& rhs)
            {
                slist_base_node* prev1 = &head;
                slist_base_node* prev2 = &rhs.head;
                iterator first1 = begin();
                iterator first2 = rhs.begin();
                iterator last1 = end();
                iterator last2 = rhs.end();
                while(first1 != last1 && first2 != last2)
                {
                    if(*first2 < *first1)
                    {
                        splice_after(iterator(static_cast<slist_node*>(prev1)), iterator(static_cast<slist_node*>(prev2)), first2++);
                        prev1 = prev1->next;
                    }
                    else
                    {
                        ++first1;
                        prev1 = prev1->next;
                    }
                }
                if(first2 != last2)
                {
                    splice_after(iterator(static_cast<slist_node*>(prev1)), rhs);
                }
            }

            void splice(iterator position, slist& rhs)
            {
                _slist_splice_after(_slist_previous(&head, position.node), &rhs.head, _slist_previous(&rhs.head, NULL));
            }

            void splice(iterator position, slist& rhs, iterator i)
            {
                iterator temp = i;
                if(temp == position || ++temp == position)
                    return;

                _slist_splice_after(_slist_previous(&head, position.node), _slist_previous(&rhs.head, i.node), i.node);
            }

            void splice_after(iterator position, iterator before_first, iterator before_last)
            {
                _slist_splice_after(position.node, before_first.node, before_last.node);
            }

            void splice_after(iterator position, slist& rhs)
            {
                _slist_splice_after(position.node, &rhs.head, _slist_previous(&rhs.head, NULL));
            }

            void swap(slist& rhs)
            {
                using MiniStl::swap;
                swap(head, rhs.head);
            }

            bool operator==(const slist& rhs) const
            {
                if(size() != rhs.size())
                    return false;

                const_iterator first1 = begin();
                const_iterator last1 = end();
                const_iterator first2 = rhs.begin();
                const_iterator last2 = rhs.end();

                for(; first1 != last1 && first2 != last2; ++first1, ++first2)
                {
                    if(*first1 != *first2)
                        return false;
                }

                return true;
            }

            bool operator!=(const slist& rhs) const
            {
                return !(operator==(rhs));
            }

        private:
            void _slist_uninitialized_fill_n(size_type n, const value_type& value);

            void destory_and_deallocate();

            void destroy_node(slist_base_node* ptr);

            slist_node* create_node(const value_type& value);

            slist_base_node* back_aux();
    };

    template<class T, class Alloc>
    void swap(slist<T, Alloc>& lhs, slist<T, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

    template<class T, class Alloc>
    bool operator==(const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Alloc>
    bool operator!=(const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class T, class Alloc>
    void slist<T, Alloc>::_slist_uninitialized_fill_n(size_type n, const value_type& value)
    {
        if(n != 0)
        {
            slist_node* first_node = create_node(value);
            slist_node* prev_node = first_node;
            --n;
            for(; n > 0; --n)
            {
                slist_node* new_node = create_node(value);
                prev_node->next = static_cast<slist_base_node*>(new_node);
                prev_node = new_node;
            }
            head.next = static_cast<slist_base_node*>(first_node);
            prev_node->next = NULL;
        }
    }

    template<class T, class Alloc>
    void slist<T, Alloc>::destory_and_deallocate()
    {
        slist_base_node* curr = head.next;

        while(curr != NULL)
        {
            slist_base_node* next = curr->next;
            destroy_node(curr);
            curr = next;
        }
        head.next = NULL;
    }

    template<class T, class Alloc>
    void slist<T, Alloc>::destroy_node(slist_base_node* ptr)
    {
        destory(static_cast<slist_node*>(ptr));
        slist_node_allocator::deallocate(static_cast<slist_node*>(ptr));
    }
    
    template<class T, class Alloc>
    typename slist<T, Alloc>::slist_node* slist<T, Alloc>::create_node(const value_type& value)
    {
        slist_node *newNode = slist_node_allocator::allocate();

        construct(&(newNode->data), value);
        return newNode;
    }

    template<class T, class Alloc>
    typename slist<T, Alloc>::slist_base_node* slist<T, Alloc>::back_aux()
    {   
        slist_base_node* first = head.next;
        slist_base_node* prev = NULL;

        while(first != NULL)
        {
            prev = first;
            first = first->next;
        }

        return prev;
    }
}

#endif
