#ifndef __MINISTL_MULTIMAP_H
#define __MINISTL_MULTIMAP_H

#include"Functional.h"
#include"Rbtree.h"
#include"Utility.h"

namespace MiniStl
{
    template<class Key, class T, class Compare = less<Key>, class Alloc = myAlloc>
    class multimap
    {
        public:
            typedef Key key_type;
            typedef pair<const key_type, T> value_type;
            typedef Compare key_compare;

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
            typedef typename rep_type::iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;

        public:
            struct value_comp
            {
                public:
                    Compare comp;

                    value_comp(const Compare& c) : comp(c) {}
                    
                    bool operator()(const value_type& lhs, const value_type& rhs)
                    {
                        return comp(lhs.first, rhs.first);
                    }
            };

        public:
            multimap() : t() {}

            multimap(const Compare& comp) : t(comp) {}

            template<class InputIterator>
            multimap(InputIterator first, InputIterator last)
            {
                t.insert_equal(first, last);
            }

            multimap(const multimap& other) : t(other.t) {}

        public:
            //iterator
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
            
            // observers

            key_compare key_comp() const
            {
                return t.key_compare();
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

            //modifiers
            iterator insert(const value_type& value)
            {
                return t.insert_equal(value);
            }

            template<class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                t.insert_equal(first, last);
            }

            iterator insert(iterator position, const value_type& value)
            {
                return t.insert_equal(position, value);
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
                return t.erase(first, last);
            }

            void clear()
            {
                t.clear();
            }

            // operator
            iterator find(const key_type& key)
            {
                return t.find(key);
            }

            iterator lower_bound(const key_type& value) const
            {
                return t.lower_bound(value);
            }

            iterator upper_bound(const key_type& value) const
            {
                return t.upper_bound(value);
            }

            pair<iterator, iterator> equal_range(const key_type& value) const
            {
                return t.equal_range(value);
            }

            size_type count(const value_type& value) const
            {
                return t.count(value);
            }

            bool operator==(const multimap& rhs) const
            {
                return t == rhs.t;
            }

            bool operator!=(const multimap& rhs) const
            {
                return !(operator==(rhs));
            }

            void swap(multimap& rhs)
            {
                using MiniStl::swap;

                swap(t, rhs.t);
            }
    };

    template<class Key, class T, class Compare, class Alloc>
    bool operator==(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator!=(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    void swap(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif
