#ifndef __MINISTL_HASHTABLE_H
#define __MINISTL_HASHTABLE_H

#include"Iterator.h"
#include"Alloc.h"
#include"Vector.h"
#include"Algobase.h"
#include"Construct.h"
#include<iostream>

namespace MiniStl
{
    static const int _num_primes = 28;
    
    static const unsigned long _prime_list[_num_primes] =
    {
        53, 97, 193, 389, 769,
        1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433,
        1572869, 3145739, 6291469, 12582917, 25165843,
        50331653, 100663319, 201326611, 402653189, 805306457,
        1610612741, 3221225473ul, 4294967291ul
    };

    inline unsigned long next_prime(unsigned long n)
    {
        const unsigned long* first = _prime_list;
        const unsigned long* last = _prime_list + _num_primes;
        const unsigned long* position = lower_bound(first, last, n);
        return position == last? *(last - 1) : *position;
    }

    template<class Value>
    struct _hash_table_node
    {
        Value value;
        _hash_table_node *next;
    };

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc> struct _hash_table_iterator;
    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc> struct _hash_table_const_iterator;
    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc = myAlloc> struct hash_table;

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc> struct _hash_table_iterator
    {
        typedef hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> hash_table;
        typedef _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
        typedef _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef _hash_table_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef Value& reference;
        typedef Value* pointer;

        node* curr;
        hash_table* ht;
        
        // constructor
        _hash_table_iterator() {}
        _hash_table_iterator(node* n, hash_table* t) : curr(n), ht(t) {}
        _hash_table_iterator(const _hash_table_iterator& rhs) : curr(rhs.curr), ht(rhs.ht) {}

        // operator
        _hash_table_iterator& operator++();
        _hash_table_iterator operator++(int);
        
        reference operator*()
        {
            return curr->value;
        }
        
        pointer operator->()
        {
            return &(operator*());
        }

        bool operator==(const iterator& rhs) const
        {
            return curr == rhs.curr;
        }

        bool operator!=(const iterator& rhs) const
        {
            return !(operator==(rhs));
        }
    };

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc> struct _hash_table_const_iterator
    {
        typedef hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> hash_table;
        typedef _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
        typedef _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef _hash_table_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef const Value& reference;
        typedef const Value* pointer;

        const node* curr;
        const hash_table* ht;

        _hash_table_const_iterator() {}
        _hash_table_const_iterator(const node* n, const hash_table* t) : curr(n), ht(t) {}
        _hash_table_const_iterator(const _hash_table_const_iterator& rhs) : curr(rhs.curr), ht(rhs.ht) {}
        _hash_table_const_iterator(const iterator& rhs) : curr(rhs.curr), ht(rhs.ht) {}

        // operator
        _hash_table_const_iterator& operator++();
        _hash_table_const_iterator operator++(int);

        reference operator*()
        {
            return curr->value;
        }

        pointer operator->()
        {
            return &(operator*());
        }

        bool operator==(const const_iterator& rhs) const
        {
            return curr == rhs.curr;
        }

        bool operator!=(const const_iterator& rhs) const
        {
            return !(operator==(rhs));
        }
    };

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct hash_table
    {
        friend _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>;
        friend _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>;
        public:
            typedef Key key_type;
            typedef Value value_type;
            typedef HashFcn hasher;
            typedef EqualKey key_equal;
            typedef ExtractKey key_of_value;

        public:
            typedef value_type& reference;
            typedef const value_type* pointer;
            typedef const value_type& const_reference;
            typedef const value_type& const_pointer;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
            typedef _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;

        private:
            typedef _hash_table_node<Value> node;
            typedef simple_alloc<node, Alloc> node_allocator;
            
            vector<node*, Alloc> buckets;
            size_type num_elements;
            hasher hash;
            key_equal equals;
            key_of_value get_key;
            
        public:
            hash_table(size_type n, const hasher &hs, const key_of_value &kov, const key_equal &ke)
            : buckets(next_prime(n), static_cast<node*>(NULL)), num_elements(0), hash(hs), equals(ke), get_key(kov) {}

            hash_table(size_type n, const hasher &hs, const key_equal &ke)
            : buckets(next_prime(n), static_cast<node*>(NULL)), num_elements(0), hash(hs), equals(ke), get_key() {}

            hash_table(const hash_table& rhs) : num_elements(rhs.num_elements), hash(rhs.hash), equals(rhs.equals), get_key(rhs.get_key)
            {
                copy_from(rhs);
            }

            ~hash_table()
            {
                clear();
            }

            // iterator
            iterator begin()
            {
                size_type total = buckets.size();
                
                for(size_type n = 0; n < total; ++n)
                {
                    if(buckets[n] != NULL)
                        return iterator(buckets[n], this);
                }

                return end();
            }

            const_iterator begin() const
            {
                size_type total = buckets.size();

                for(size_type n = 0; n < total; ++n)
                {
                    if(buckets[n] != NULL)
                        return const_iterator(buckets[n], this);
                }

                return end();
            }

            const_iterator cbegin() const
            {
                return begin();
            }

            iterator end()
            {
                return iterator(static_cast<node*>(NULL), this);
            }

            const_iterator end() const
            {
                return const_iterator(static_cast<node*>(NULL), this);
            }

            const_iterator cend() const
            {
                return begin();
            }

            // capacity
            size_type size() const
            {
                return num_elements;
            }

            bool empty() const
            {
                return num_elements == 0;
            }

            size_type bucket_num() const
            {
                return buckets.size();
            }

            size_type elems_in_bucket(size_type n) const
            {
                node* first = buckets[n];
                size_type count = 0;

                for(; first != NULL; ++first, ++count)
                    ;

                return count;
            }

            //modifier
            pair<iterator, bool> insert_unique(const value_type& value)
            {
                resize(num_elements + 1);
                return insert_unique_noresize(value);
            }

            iterator insert_equal(const value_type& value)
            {
                resize(num_elements + 1);
                return insert_equal_noresize(value);
            }

            template<class InputIterator>
            void insert_unique(InputIterator first, InputIterator last)
            {
                insert_unique(first, last, iterator_category(first));
            }
        
            // input_iterator无法优化
            template<class InputIterator>
            void insert_unique(InputIterator first, InputIterator last, input_iterator_tag)
            {
                for(; first != last; ++first)
                {
                    resize(num_elements + 1);
                    insert_unique_noresize(*first);
                }
            }

            // 对forward_iterator优化
            template<class ForwardIterator>
            void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
            {
                typedef typename _iterator_traits<ForwardIterator>::difference_type Distance;

                Distance n = distance(first, last);
                resize(num_elements + n);
                for(; first != last; ++first)
                    insert_unique_noresize(*first);
            }

            template<class InputIterator>
            void insert_equal(InputIterator first, InputIterator last)
            {
                insert_equal(first, last, iterator_category(first));
            }

            // 对input_iterator无法优化
            template<class InputIterator>
            void insert_equal(InputIterator first, InputIterator last, input_iterator_tag)
            {
                for(; first != last; ++first)
                {
                    resize(num_elements + 1);
                    insert_equal_noresize(*first);
                }
            }

            // 对forward_iterator优化
            template<class ForwardIterator>
            void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
            {
                typedef typename _iterator_traits<ForwardIterator>::difference_type Distance;

                Distance n = distance(first, last);
                resize(num_elements + n);
                for(; first != last; ++first)
                    insert_equal_noresize(*first);
            }

            void erase(iterator position)
            {
                if(node* target = position.curr)
                {
                    size_type n = bkt_num(target->value);
                    node* first = buckets[n];

                    if(target == first)
                    {
                        buckets[n] = first->next;
                        destroy_node(first);
                        --num_elements;
                    }
                    else
                    {
                        while(first->next != target)
                            first = first->next;
                        first->next = target->next;
                        destroy_node(target);
                        --num_elements;
                    }
                }
            }

            void erase(const_iterator position)
            {
                if(const node *temp = position.curr)
                {
                    node* target = const_cast<node*>(temp);
                    size_type n = bkt_num(target->value);
                    node* first = buckets[n];
                    if(first == target)
                    {
                        buckets[n] = first->next;
                        destroy_node(target);
                        --num_elements;
                    }
                    else
                    {
                        while(first->next != target)
                            first = first->next;
                        first->next = target->next;
                        destroy_node(target);
                        --num_elements;
                    }
                }
            }

            // 无法优化, 因为[first, last)可能在几个bucket当中
            void erase(iterator first, iterator last)
            {
                if(first == begin() && last == end())
                    clear();
                else
                {
                    while(first != last)
                        erase(first++);
                }
            }

            size_type erase(const key_type& value)
            {
                size_type count = 0;

                size_type n = bkt_num_key(value);
                node* first = buckets[n];
                // 先不对头节点处理
                if(first)
                {
                    node* curr = first;
                    node* next = curr->next;
                    while(next != NULL)
                    {
                        if(equals(value, get_key(next->value)))
                        {
                            curr->next = next->next;
                            destroy_node(next);
                            next = curr->next;
                            ++count;
                            --num_elements;
                        }
                        else
                        {
                            curr = next;
                            next = next->next;
                        }
                    }
                }
                // 对头节点处理
                if(first)
                {
                    if(equals(value, get_key(first->value)))
                    {
                        buckets[n] = first->next;
                        destroy_node(first);
                        ++count;
                        --num_elements;
                    }
                }
                return count;
            }

            void clear()
            {
                size_type n = buckets.size();
                for(size_type i = 0; i < n; ++i)
                {
                    node* first = buckets[i];
                    while(first != NULL)
                    {
                        buckets[i] = first->next;
                        destroy_node(first);
                        first = buckets[i];
                    }
                }
                num_elements = 0;
                // 以下为了防止distance(first(), end()) != size()
                fill(buckets.begin(), buckets.end(), static_cast<node*>(NULL));
            }

            // operator
            iterator find(const key_type& key)
            {
                size_type n = bkt_num_key(key);

                node* first = buckets[n];
                while(first != NULL)
                {
                    if(equals(key, get_key(first->value)))
                        return iterator(first, this);
                }

                return end();
            }

            size_type count(const key_type& key)
            {
                size_type count = 0;

                size_type n = bkt_num_key(key);
                node* first = buckets[n];
                while(first)
                {
                    if(equals(key, get_key(first->value)))
                        ++count;
                    first = first->next;
                }

                return count;
            }

            bool operator==(const hash_table& rhs) const
            {
                if(size() != rhs.size())
                    return false;

                const_iterator first1 = begin();
                const_iterator first2 = rhs.begin();
                const_iterator end1 = end();
                const_iterator end2 = rhs.end();

                for(; first1 != end1 && first2 != end2; ++first1, ++first2)
                    if(*first1 != *first2)
                        return false;

                return true;
            }
            
            bool operator!=(const hash_table& rhs) const
            {
                return !(operator==(rhs));
            }

            void swap(hash_table& rhs)
            {
                using MiniStl::swap;
                swap(buckets, rhs.buckets);
                swap(num_elements, rhs.num_elements);
                swap(hash, rhs.hash);
                swap(equals, rhs.equals);
                swap(get_key, rhs.get_key);
            }

        private:
            node* create_node(const value_type& val)
            {
                node* temp = node_allocator::allocate();
                construct(&(temp->value), val);
                temp->next = NULL;
                return temp;
            }

            void destroy_node(node* p)
            {
                destory(&(p->value));
                node_allocator::deallocate(p);
            }

            void resize(size_type n)
            {
                const size_type old_size = buckets.size();
                const size_type new_size = next_prime(n);
                // 桶内元素与桶数比例为1
                if(new_size > old_size)
                {
                    vector<node*, Alloc> temp(new_size, static_cast<node*>(NULL));
                    for(size_type n = 0; n < old_size; ++n)
                    {
                        node* first = buckets[n];
                        while(first != NULL)
                        {
                            size_type new_bucket_num = bkt_num(first->value, new_size);
                            buckets[n] = first->next;
                            first->next = temp[new_bucket_num];
                            temp[new_bucket_num] = first;
                            first = buckets[n];
                        }
                    }
                    buckets.swap(temp);
                }
            }

            pair<iterator, bool> insert_unique_noresize(const value_type& val)
            {
                const size_type n = bkt_num(val);   
                node* first = buckets[n];

                for(; first != NULL; first = first->next)
                {
                    if(equals(get_key(first->value), get_key(val)))
                        return make_pair(iterator(first, this), false);
                }

                node* temp = create_node(val);
                temp->next = buckets[n];
                buckets[n] = temp;
                ++num_elements;
                return make_pair(iterator(temp, this), true);
            }

            iterator insert_equal_noresize(const value_type& val)
            {
                const size_type n = bkt_num(val);
                node* first = buckets[n];

                for(; first != NULL; first = first->next)
                {
                    if(equals(get_key(first->value), get_key(val)))
                    {
                        node* temp = create_node(val);
                        temp->next = first->next;
                        first->next = temp;
                        ++num_elements;
                        return iterator(temp, this);
                    }
                }

                node* temp = create_node(val);
                temp->next = buckets[n];
                buckets[n] = temp;
                ++num_elements;
                return iterator(temp, this);
            }

            void copy_from(const hash_table& rhs)
            {
                size_type n = rhs.bucket_num();
                buckets.clear();
                buckets.reserve(n);
                for(size_type i = 0; i < n; ++i)
                    buckets.insert(buckets.end(), static_cast<node*>(NULL));
                
                for(size_type i = 0; i < n; ++i)
                {
                    if(node* first = rhs.buckets[i])
                    {
                        node* first_new_node = create_node(first->value);
                        node* last_new_node = first_new_node;
                        buckets[i] = first_new_node;

                        while(first->next)
                        {
                            first = first->next;
                            node* temp = create_node(first->value);
                            last_new_node->next = temp;
                            last_new_node = temp;
                        }
                        last_new_node->next = NULL;
                    }

                    // 以下方法会造成反链
                    /*node* first = rhs.buckets[i];

                    while(first != NULL)
                    {
                        node* temp = create_node(first->value);
                        temp->next = buckets[i];
                        buckets[i] = temp;
                        first = first->next;
                    }*/
                }
                num_elements = rhs.num_elements;
            }

            size_type bkt_num(const value_type& value) const
            {
                return bkt_num_key(get_key(value));
            }

            size_type bkt_num(const value_type& value, size_type n) const
            {
                return bkt_num_key(get_key(value), n);
            }

            size_type bkt_num_key(const key_type& value, size_type n) const
            {
                return hash(value) % n;
            }

            size_type bkt_num_key(const key_type& value) const
            {
                return bkt_num_key(value, bucket_num());
            }
    };

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>&
    _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
    {
        node* temp = curr;
        curr = curr->next;

        if(temp->next != NULL)
            return *this;
        else
        {
            size_type total = ht->bucket_num();
            size_type n = ht->bkt_num(temp->value);
            while(++n < total)
            {
                if(ht->buckets[n] != NULL)
                {
                    curr = ht->buckets[n];
                    return *this;
                }
            }
        }

        return *this;
    }

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>
    _hash_table_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
    {
        iterator temp = *this;
        operator++();
        return temp;
    }

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>&
    _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
    {
        const node* temp = curr;
        curr = curr->next;

        if(temp->next != NULL)
        {
            return *this;
        }
        else
        {
            size_type total = ht->bucket_num();
            size_type n = ht->bkt_num(temp->value);

            while(++n < total)
            {
                if(ht->buckets[n] != NULL)
                {
                    curr = ht->buckets[n];
                    return *this;
                }
            }
        }

        return *this;
    }

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>
    _hash_table_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
    {
        const_iterator temp = *this;
        operator++();
        return temp;
    }

    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    bool operator==(const hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>& lhs, 
                    const hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>& rhs)
    {
        return rhs.operator==(lhs);
    }


    template<class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    bool operator!=(const hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>& lhs, 
                    const hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>& rhs)
    {
        return !(operator==(lhs, rhs));
    }
}
#endif
