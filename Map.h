#ifndef __MINISTL_MAP_H
#define __MINISTL_MAP_H
#include"Rbtree.h"
#include"Functional.h"
#include"Utility.h"

namespace MiniStl
{
    template<class Key, class T, class Compare = less<Key>, class Alloc = myAlloc>
    class map
    {
        public:
            typedef Key key_type;
            typedef pair<const key_type, T> value_type;
        
        private:
            typedef rb_tree<key_type, value_type, select1st<value_type>, Compare, Alloc> rep_type;
            rep_type t;

        public:
            typedef typename rep_type::pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::reference reference;
            typedef typename rep_type::const_reference const_reference;
            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference_type;
            typedef typename rep_type::const_iterator const_iterator;
            typedef typename rep_type::iterator iterator;
            typedef Compare key_compare;

        public:
            struct value_compare
            {
                private:
                    Compare comp;
                
                public:
                    value_compare(const Compare& c) : comp(c) {}
                    
                    bool operator()(const value_type& p1, const value_type& p2)
                    {
                        return comp(p1.first, p2.first);
                    }
            };

        public:
            map() : t() {}
            map(Compare comp) : t(comp) {}
            
            template<class InputIterator>
            map(InputIterator first, InputIterator last) : t(first, last) {}

            // iterator
            iterator begin()
            {
                return t.begin();
            }

            const_iterator begin() const
            {
                return t.begin();
            }

            const_iterator cbegin() const
            {
                return t.cbegin();
            }

            iterator end()
            {
                return t.end();
            }

            const_iterator end() const
            {
                return t.end();
            }

            const_iterator cend() const
            {
                return t.cend();
            }

            // element access
            T& operator[](const key_type& k)
            {
                return (*(t.insert_unique(value_type(k, T())).first)).second;
            }

            // observers
            key_compare key_comp() const
            {
                return t.key_compare();
            }

            value_compare value_comp() const
            {
                return value_compare(key_comp());
            }
            
            // capacity
            bool empty() const
            {
                return t.empty();
            }

            size_type size() const
            {
                return t.size();
            }

            // modifier
            pair<iterator, bool> insert(const value_type& value)
            {
                return t.insert_unique(value);
            }

            pair<iterator, bool> insert(iterator position, const value_type& value)
            {
                return t.insert_unique(position, value);
            }

            template<class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                t.insert_unique(first, last);
            }

            iterator erase(iterator position)
            {
                return t.erase(position);
            }

            size_type erase(const key_type& value)
            {
                return t.erase(value);
            }

            void erase(iterator first, iterator last)
            {
                t.erase(first, last);
            }

            void clear()
            {
                t.clear();
            }

            void swap(map& rhs)
            {
                using MiniStl::swap;

                swap(t, rhs.t);
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

            iterator lower_bound(const key_type& value)
            {
                return t.lower_bound(value);
            }

            iterator upper_bound(const key_type& value)
            {
                return t.upper_bound(value);
            }

            pair<iterator, iterator> equal_range(const key_type& value)
            {
                return t.equal_range(value);
            }

            bool operator==(const map& rhs) const
            {
                return t == rhs.t;
            }

            bool operator!=(const map& rhs) const
            {
                return !(operator==(rhs));
            }
    };

    template<class Key, class T, class Compare, class Alloc>
    bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<class Key, class T, class Compare, class Alloc>
    void swap(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif
