#ifndef __MINISTL_MULTISET_H
#define __MINISTL_MULTISET_H

#include"Stl_function.h"
#include"Rbtree.h"
#include"Functional.h"
#include<cassert>

namespace MiniStl
{
    template<class T, class Compare = less<T>, class Alloc = myAlloc>
    class multiset
    {
        public:
            typedef T value_type;
            typedef T key_type;
        
        private:
            typedef rb_tree<value_type, key_type, identity<value_type>, Compare, Alloc> rp_type; 
            rp_type t;

        public:
            typedef typename rp_type::const_reference reference;
            typedef typename rp_type::const_reference const_reference;
            typedef typename rp_type::const_pointer pointer;
            typedef typename rp_type::const_pointer const_pointer;
            typedef typename rp_type::size_type size_type;
            typedef typename rp_type::difference_type difference_type;
            typedef typename rp_type::const_iterator iterator;
            typedef typename rp_type::const_iterator const_iterator;
            typedef Compare value_compare;
            typedef Compare key_compare;

        public:
            multiset() : t() {}
            multiset(const Compare& comp) : t(comp) {}
            
            template<class InputIterator>
            multiset(InputIterator first, InputIterator last)
            {
                t.insert_equal(first, last);
            }
        
        public:
            //iterator
            iterator begin() const
            {
                return t.begin();
            }

            iterator cbegin() const
            {
                return t.cbegin();
            }

            iterator end() const
            {
                return t.end();
            }

            iterator cend() const
            {
                return t.cend();
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

            // modifier
            iterator insert(const value_type& value)
            {
                return t.insert_equal(value);
            }
            
            template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                t.insert_equal(first, last);
            }

            iterator insert(iterator position, const value_type& value)
            {
                typedef typename rp_type::iterator rp_iterator;
                return t.insert_equal((rp_iterator&)position, value);
            }

            iterator erase(iterator position)
            {
                typedef typename rp_type::iterator rp_iterator;
                return t.erase((rp_iterator&)position);
            }

            void erase(iterator first, iterator last)
            {
                typedef typename rp_type::iterator rp_iterator;
                t.erase((rp_iterator&)first, (rp_iterator&)last);
            }

            size_type erase(const value_type& value)
            {
                return t.erase(value);
            }
            
            void clear()
            {
                t.clear();
            }

            void swap(const multiset& rhs)
            {
                t.swap(rhs.t);
            }

        // operator
            iterator find(const key_type& key)
            {
                return t.find(key);
            }

            size_type count(const key_type& value) const
            {
                return t.count(value);
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

            bool operator==(const multiset& rhs) const
            {
                return t == rhs.t;
            }

            bool operator!=(const multiset& rhs) const
            {
                return !(operator==(rhs));
            }

            void swap(multiset& rhs)
            {
                using MiniStl::swap;
                swap(t, rhs.t);
            }
    };

    template<class T, class Compare, class Alloc>
    bool operator==(const multiset<T, Compare, Alloc>& lhs, const multiset<T, Compare, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Compare, class Alloc>
    bool operator!=(const multiset<T, Compare, Alloc>& lhs, const multiset<T, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class T, class Compare, class Alloc>
    void swap(multiset<T, Compare, Alloc>& lhs, multiset<T, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif
