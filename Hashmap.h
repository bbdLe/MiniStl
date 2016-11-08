#ifndef __MINISTL_HASHMAP_H
#define __MINISTL_HASHMAP_H
#include"Hashtable.h"
#include"Alloc.h"
#include"Stl_function.h"
#include"Utility.h"
#include"Functional.h"

namespace MiniStl
{
    template<class Key, class Value, class HashFcn, class EqualKey, class Alloc = myAlloc>
    class hashmap
    {
        public:
            typedef Key key_type;
            typedef pair<const Key, Value> value_type;
        
        private:
            typedef hash_table<key_type, value_type, HashFcn, select1st<value_type>, EqualKey, Alloc> hash_table;
            hash_table t;

        public:
            typedef typename hash_table::const_reference reference;
            typedef typename hash_table::const_pointer pointer;
            typedef typename hash_table::const_iterator iterator;
            typedef typename hash_table::size_type size_type;
            typedef typename hash_table::difference_type difference_type;
            typedef typename hash_table::const_reference const_reference;
            typedef typename hash_table::const_iterator const_iterator;
            typedef typename hash_table::const_pointer const_pointer;
        
        public:
            // constructor
            hashmap(const HashFcn& hs, const EqualKey& eq, const size_type n = 100) : t(n, hs, eq) {}
            hashmap(const hashmap& rhs) : t(rhs.t) {}
    
            // iterator
            iterator begin() const
            {
                return t.begin();
            }

            iterator end() const
            {
                return t.end();
            }

            iterator cend() const
            {
                return t.cend();
            }

            iterator cbegin() const
            {
                return t.cbegin();
            }

            // modifier
            pair<iterator, bool> insert(const value_type& value)
            {
                return t.insert_unique(value);
            }
            
            template<class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                t.insert_unique(first, last);
            }

            void erase(iterator position)
            {
                t.erase(position);
            }

            size_type erase(const key_type& value)
            {
                return t.erase(value);
            }

            void clear()
            {
                t.clear();
            }

            // capacity
            size_type size() const
            {
                return t.size();
            }

            bool empty() const
            {
                return t.empty();
            }

            size_type elems_in_bucket(size_type n) const
            {
                return t.elems_in_bucket(n);
            }

            size_type bucket_num() const
            {
                return t.bucket_num();
            }

            // operator
            iterator find(const key_type& key)
            {
                return t.find(key);
            }

            size_type count(const key_type& value)
            {
                return t.count(value);
            }

            bool operator==(const hashmap& rhs)
            {
                return rhs.t == t;
            }

            bool operator!=(const hashmap& rhs)
            {
                return !(operator==(rhs));
            }

            void swap(hashmap& rhs)
            {
                t.swap(rhs.t);
            }
    };

    template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    bool operator==(const hashmap<Key, Value, HashFcn, EqualKey, Alloc>& lhs, const hashmap<Key, Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    bool operator!=(const hashmap<Key, Value, HashFcn, EqualKey, Alloc>& lhs, const hashmap<Key, Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    void swap(hashmap<Key, Value, HashFcn, EqualKey, Alloc>& lhs, hashmap<Key, Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}


#endif
