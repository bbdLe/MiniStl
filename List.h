#ifndef __MINISTL_LIST_H
#define __MINISTL_LIST_H

#include"Iterator.h"
#include"Allocator.h"
#include<cstddef>
#include"Utility.h"

namespace MiniStl
{
    // 链表节点
    template<class T>
    struct _list_node
    {
        _list_node *next;
        _list_node *prev;
        T data;
    };

    template<class T, class Pointer, class Reference>
    struct _list_iterator
    {
        typedef bidirectional_iterator_tag iterator_category; // (1)
        typedef T value_type;               // (2)
        typedef Pointer pointer;            // (3)
        typedef Reference reference;        // (4)
        typedef _list_node<T>* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;  // (5)
        typedef _list_iterator self;
        typedef _list_iterator<T, T *, T &> iterator;

        // data
        link_type node;
        
        // constructor
        _list_iterator() {}

        _list_iterator(link_type x) : node(x) {}

        _list_iterator(const self& rhs) : node(rhs.node) {}

        // operator
        self& operator=(const self& rhs)
        {
            node = rhs.node;
            return *this;
        }

        bool operator==(const self& rhs) const
        {
            return node == rhs.node;
        }

        bool operator!=(const self& rhs) const
        {
            return !operator==(rhs);
        }

        self& operator++()
        {
            node = node->next;
            return *this;
        }

        self operator++(int)
        {
            self temp = *this;
            ++(*this);
            return temp;
        }

        self& operator--()
        {
            node = node->prev;
            return *this;
        }

        self operator--(int)
        {
            self temp = *this;
            --(*this);
            return temp;
        }

        reference operator*()
        {
            return node->data;
        }

        pointer operator->()
        {
            return &(operator*());
        }
    };
    
    template<class T, class Pointer, class Reference>
    bool operator==(const _list_iterator<T, Pointer, Reference>& lhs, const _list_iterator<T, Pointer, Reference>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Pointer, class Reference>
    bool operator!=(const _list_iterator<T, Pointer, Reference>& lhs, const _list_iterator<T, Pointer, Reference>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class T, class Alloc = myAlloc>
    class list
    {
        private:
            typedef _list_node<T> list_node;
            typedef list_node* link_type;
            typedef simple_alloc<list_node, Alloc> list_node_allocator;
            link_type node;

        public:
            typedef T value_type;
            typedef value_type& reference;
            typedef value_type* pointer;
            typedef const value_type& const_reference;
            typedef const value_type* const_pointer;
            typedef _list_iterator<value_type, pointer, reference> iterator;
            typedef _list_iterator<value_type, const_pointer, const_reference> const_iterator;
            typedef reverser_iterator<iterator> reverse_iterator;
            typedef reverser_iterator<const_iterator> const_reverse_iterator;
            typedef ptrdiff_t difference_type;
            typedef size_t size_type;

        public:
            // constructor
            list()
            {
                empty_initialize();
            }

            template<class InputIterator>
            list(InputIterator first, InputIterator end)
            {
                range_initialize(first, end);
            }

            list(size_type n, const T& value)
            {
                empty_initialize();
                insert(begin(), n, value);
            }

            list(const list& rhs)
            {
                range_initialize(rhs.begin(), rhs.end());
            }

            // destructor
            ~list()
            {
                clear();
                destory_node(node);
            }

            // operator=
            list& operator=(const list& rhs)
            {
                clear();
                insert(begin(), rhs.begin(), rhs.end());
                return *this;
            }

            // iterator
            iterator begin()
            {
                return iterator(node->next);
            }

            reverse_iterator rbegin()
            {
                return reverse_iterator(end());
            }

            const_reverse_iterator rbegin() const
            {
                return const_reverse_iterator(end());
            }

            const_iterator begin() const
            {
                return const_iterator(node->next);
            }

            const_iterator cbegin() const
            {
                return const_iterator(node->next);
            }

            iterator end()
            {
                return iterator(node);
            }

            reverse_iterator rend()
            {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rend() const
            {
                return reverse_iterator(begin());
            }

            const_iterator end() const
            {
                return const_iterator(node);
            }

            const_iterator cend() const
            {
                return const_iterator(node);
            }

            // capacity, 参考条款4
            bool empty() const
            {
                return node->next == node;
            }

            // list的size函数为O(n)
            size_type size() const
            {
                size_type count = 0;
                for(auto beg = begin(); beg != end(); ++beg, ++count)
                    ;
                return count;
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
                return *(--end());
            }

            const_reference back() const
            {
                return *(--end());
            }

            // modifiers
            // list的区间insert有一定的速度优势, 修改指针的次数比较少
            template<class InputIterator>
            void insert(iterator result, InputIterator first, InputIterator end)
            {
                if(first != end)
                {
                    link_type first_node = create_node(*first);
                    link_type last_node = first_node;
                    ++first;
                    for(; first != end; ++first)
                    {
                        link_type curr_node = create_node(*first);
                        last_node->next = curr_node;
                        curr_node->prev = last_node;
                        last_node = curr_node;
                    }
                    first_node->prev = result.node->prev;
                    result.node->prev->next = first_node;
                    last_node->next = result.node;
                    result.node->prev = last_node;
                }
                //for(; first != end; ++first)
                //  insert(result, *first);     // list的insert不会引起迭代器的失效, 所以不需要更新迭代器
            }

            // 细节如上
            void insert(iterator result, size_type n, const T& value)
            {
                if(n != 0)
                {
                    link_type first_node = create_node(value);
                    link_type last_node = first_node;
                    --n;
                    for(; n != 0; --n)
                    {
                        link_type curr_node = create_node(value);
                        last_node->next = curr_node;
                        curr_node->prev = last_node;
                        last_node = curr_node;
                    }
                    first_node->prev = result.node->prev;
                    result.node->prev->next = first_node;
                    last_node->next = result.node;
                    result.node->prev = last_node;
                }
                //for(; n != 0; --n)
                //  insert(result, value);
            }

            iterator insert(iterator result, const T& value)
            {
                link_type temp = create_node(value);
                temp->next = result.node;           // 双向链表插入总共要更新四个指针:)
                temp->prev = result.node->prev;
                result.node->prev->next = temp;
                result.node->prev = temp;
                return temp;
            }

            iterator erase(iterator position)
            {
                link_type nextNode = position.node->next;
                link_type prevNode = position.node->prev;
                nextNode->prev = prevNode;          // 双向链表删除只需要更新两个指针即可:)
                prevNode->next = nextNode;
                destory_node(position.node);
                return iterator(nextNode);
            }

            // 区间函数, 相比多次调用erase有一定速度优势, 参考条款5
            iterator erase(iterator first, iterator end)
            {
                link_type prevNode = first.node->prev;
                link_type nextNode = end.node;
                prevNode->next = nextNode;
                nextNode->prev = prevNode;
                while(first != end)
                    destory_node((first++).node);
                return end;
            }

            void push_back(const T& value)
            {
                insert(end(), value);
            }

            void push_front(const T& value)
            {
                insert(begin(), value);
            }
            
            void pop_back()
            {
                erase(--end());
            }

            void pop_front()
            {
                erase(begin());
            }

            void clear()
            {
                link_type curr = node->next;
                while(curr != node)
                {
                    link_type next = curr->next;
                    destory_node(curr);
                    curr = next;
                }
                node->prev = node;
                node->next = node;
            }

            void resize(size_type n)
            {
                resize(n, T());
            }

            void resize(size_type n, const T& value)
            {
                iterator iter = begin();
                size_type count = 0;
                for(; iter != end() && count != n; ++count, ++iter)
                    ;
                if(count == n)
                {
                    erase(iter, end());
                }
                else
                {
                    insert(end(), n - count, value);
                }
            }

            void swap(list &rhs)
            {
                using MiniStl::swap;
                swap(node, rhs.node);
            }

            // Operator
            void splice(iterator position, list& x)
            {
                if(!x.empty())
                    transfer(position, x.begin(), x.end());
            }

            void splice(iterator position, list&, iterator i)
            {
                iterator j = i;
                ++j;
                if(i == position || j == position)      // 元素本身就在position前面或者元素就是poisition, 两种情况直接返回
                    return;
                transfer(position, i, j);
            }

            void splice(iterator position, iterator first, iterator end)
            {
                if(first != end)
                    transfer(position, first, end);
            }

            // 链表x必须已经排序!
            void merge(list& x)
            {
                if(!x.empty())
                {
                    iterator first1 = begin();
                    iterator end1 = end();
                    iterator first2 = x.begin();
                    iterator end2 = x.end();
                    while(first1 != end1 && first2 != end2)
                    {
                        if(*first1 > *first2)
                        {
                            iterator next = first2;
                            transfer(first1, first2, ++next);
                            first2 = next;
                        }
                        else
                        {
                            ++first1;
                        }
                    }
                    if(first2 != end2)
                        splice(end(), first2, end2);
                }
            }

            // 先排序再unique
            void unique()
            {
                // 没有元素或者只有一个元素
                if(node->next == node || node->next->next == node)
                    return;
                iterator first = begin();
                iterator last = end();
                iterator next = first;
                ++next;
                while(next != last)
                {
                    if(*first == *next)
                    {
                        erase(next);
                        next = first;
                    }
                    else
                    {
                        first = next;
                    }
                    ++next;
                }
            }

            void remove(const value_type &value)
            {
                if(!empty())
                {
                    iterator curr = begin();
                    iterator last = end();
                    while(curr != last)
                    {
                        if(*curr == value)
                        {
                            erase(curr++);
                        }
                        else
                        {
                            ++curr;
                        }
                    }
                }
            }

            template<class Predicate>
            void remove_if(Predicate pred)
            {
                if(!empty())
                {
                    iterator curr = begin();
                    iterator end = end();
                    while(curr != end)
                    {
                        if(pred(*curr))
                        {
                            erase(curr++);
                        }
                        else
                        {
                            ++curr;
                        }
                    }
                }
            }

            void reverse()
            {
                // only zero or one element
                if(node->next == node || node->next->next == node)
                    return;
                else
                {
                    iterator first = begin();
                    ++first;
                    while(first != end())
                    {
                        iterator old = first;
                        ++first;
                        splice(begin(), old, first);
                    }
                }
            }

            void sort()
            {
                if(node->next == node || node->next->next == node)
                    return;
                list carry;
                list  counter[64];      // 最多可排列2^64 - 1个元素
                int fill = 0;
                int i = 0;
                while(!empty())
                {
                    carry.splice(carry.begin(), *this, begin());
                    while(i < fill && !counter[i].empty())
                    {
                        counter[i].merge(carry);
                        carry.swap(counter[i++]);
                    }
                    carry.swap(counter[i]);
                    if(i == fill)
                        ++fill;
                    i = 0;
                }
                for(; i < fill; ++i)
                {
                    carry.merge(counter[i]);
                }
                carry.swap(*this);
            }

            bool operator==(const list& rhs) const
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

        private:
            link_type get_node();

            void put_node(link_type ptr);

            link_type create_node(const value_type& value);

            void destory_node(link_type ptr);

            void empty_initialize();

            template <class InputIterator>
            void range_initialize(InputIterator first, InputIterator end);

            void transfer(iterator position, iterator first, iterator end);
    };

    template<class T, class Alloc>
    typename list<T, Alloc>::link_type list<T, Alloc>::get_node()
    {
            return list_node_allocator::allocate();
    }

    template<class T, class Alloc>
    void list<T, Alloc>::put_node(link_type ptr)
    {
        list_node_allocator::deallocate(ptr);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::link_type list<T, Alloc>::create_node(const value_type& value)
    {
        link_type ptr = get_node();
        // Fixme, doesn't handle the exception
        construct(&ptr->data, value);
        return ptr;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::destory_node(link_type ptr)
    {
        destory(&ptr->data);
        put_node(ptr);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::empty_initialize()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
    
    template<class T, class Alloc> template <class InputIterator>
    void list<T, Alloc>::range_initialize(InputIterator first, InputIterator end)
    {
        empty_initialize();
        insert(begin(), first, end);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::transfer(iterator position, iterator first, iterator end)
    {
        first.node->prev->next = end.node; // 1
        end.node->prev->next = position.node; // 2
        link_type temp = position.node->prev; // 3
        temp->next = first.node; // 4
        position.node->prev = end.node->prev; // 5
        end.node->prev = first.node->prev; // 6
        first.node->prev = temp; // 7
    }

    template<class T, class alloc>
    void swap(list<T, alloc> &lhs, list<T, alloc> &rhs)
    {
        lhs.swap(rhs);
    }

    template<class T, class Alloc>
    bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Alloc>
    bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
}
#endif
