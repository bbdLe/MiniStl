#ifndef __MINISTL_SET_H
#define __MINISTL_SET_H

#include"Rbtree.h"
#include"Stl_function.h"
#include"Functional.h"

namespace MiniStl
{
    template<class T, class Compare = less<T>, class Alloc = myAlloc>
    class set
    {
        public:
            

        public:
            typedef T value_type;
            typedef T key_type;
            typedef Compare value_compare;
            typedef Compare key_compare;

        private:
            typedef rb_tree<value_type, value_type, identity<value_type>, Compare, Alloc> rep_type;
            rep_type t;
        
        public:
            typedef typename rep_type::const_iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;
            typedef typename rep_type::const_reference reference;
            typedef typename rep_type::const_reference const_reference;
            typedef typename rep_type::const_pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference;

        public:
            set() : t() {}
            set(Compare comp) : t(comp) {}
            template<class InputIterator> set(InputIterator first, InputIterator last) : t(first, last) {}

        public:
            // iterator
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
            pair<iterator, bool> insert(const value_type& value)
            {
                pair<typename rep_type::iterator, bool> p = t.insert_unique(value);
                return pair<iterator, bool>(p.first, p.second);
            }

            template<class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                t.insert_unique(first, last);
            }

            iterator insert(iterator position, const value_type& value)
            {
                typedef typename rep_type::iterator rep_iterator;
                return t.insert_unique((rep_iterator&)position, value);
            }

            iterator erase(iterator position)
            {
                typedef typename rep_type::iterator rep_iterator;
                return t.erase((rep_iterator&)position);
            }

            void erase(iterator first, iterator last)
            {
                typedef typename rep_type::iterator rep_iterator;
                t.erase((rep_iterator&)first, (rep_iterator&)(last));
            }

            size_type erase(const value_type& value)
            {
                return t.erase(value);
            }

            void clear()
            {
                t.clear();
            }
            
            // operator
            iterator find(const value_type& value)
            {
                return t.find(value);
            }

            iterator lower_bound(const value_type& value)
            {
                return t.lower_bound(value);
            }

            iterator upper_bound(const value_type& value)
            {
                return t.upper_bound(value);
            }

            pair<iterator, iterator> equal_range(const value_type& value)
            {
                pair<typename rep_type::iterator, typename rep_type::iterator> p = t.equal_range(value);
                return pair<iterator, iterator>(p.first, p.second);
            }

            Compare key_comp() const
            {
                return t.key_compare();
            }

            Compare value_comp() const
            {
                return t.key_compare();
            }

            bool operator==(const set& rhs) const
            {
                return t == rhs.t;
            }

            bool operator!=(const set& rhs) const
            {
                return !(operator==(rhs));
            }

            void swap(set& rhs)
            {
                using MiniStl::swap;
                swap(t, rhs.t);
            }
    };

    template<class T, class Compare, class Alloc>
    void swap(set<T, Compare, Alloc>& lhs, set<T, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

    template<class T, class Compare, class Alloc>
    bool operator==(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Compare, class Alloc>
    bool operator!=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
