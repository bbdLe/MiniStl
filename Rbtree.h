#ifndef __MINISTL_RBTREE_H
#define __MINISTL_RBTREE_H

#include"Iterator.h"
#include"Alloc.h"
#include"Construct.h"
#include"Utility.h"
#include"Algobase.h"
#include<iostream>

namespace MiniStl
{
    typedef bool _rb_tree_color_type;
    const _rb_tree_color_type _rb_tree_red = false;
    const _rb_tree_color_type _rb_tree_black = true;

    // 第一级节点
    struct _rb_tree_node_base
    {
        typedef _rb_tree_node_base* base_ptr;
        typedef _rb_tree_color_type color_type;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr x)
        {
            while(x->left != NULL)
                x = x->left;

            return x;
        }

        static base_ptr maximum(base_ptr x)
        {
            while(x->right != NULL)
                x = x->right;

            return x;
        }
    };


    // 第二级节点
    template<class T>
    struct _rb_tree_node : public _rb_tree_node_base
    {
        typedef _rb_tree_node* link_type;
        T value;
    };

    // 第一级迭代器
    struct _rb_tree_base_iterator
    {
        typedef _rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node;

        bool operator==(const _rb_tree_base_iterator& rhs) const
        {
            return node == rhs.node;
        }

        bool operator!=(const _rb_tree_base_iterator& rhs) const
        {
            return !(operator==(rhs));
        }

        void increment()
        {
            if(node->right != NULL)
            {
                node = node->right;
                while(node->left != NULL)
                    node = node->left;
            }
            else
            {
                base_ptr p = node->parent;

                while(p->right == node)
                {
                    node = p;
                    p = p->parent;
                }
                
                // 防止寻找根节点的下一个节点, 而根节点并无右儿子, 此时node即为header, 也就是最后一个++iterator的时候，会变成end()
                if(node->right != p)
                    node = p;
            }
        }

        void decrement()
        {
            // 发生在iterator为end的时候，此时operator--会指向最大值
            if(node->color == _rb_tree_red && node->parent->parent == node)
            {
                node = node->right;
            }
            else if(node->left != NULL)
            {
                node = node->left;
                while(node->right != NULL)
                    node = node->right;
            }
            else
            {
                base_ptr p = node->parent;

                while(p->left == node)
                {
                    node = p;
                    p = p->parent;
                }
                node = p;
            }
        }
    };

    // 第二级迭代器
    template<class T, class Pointer, class Reference>
    struct _rb_tree_iterator : public _rb_tree_base_iterator
    {
        public:
            typedef T value_type;
            typedef Pointer pointer;
            typedef Reference reference;
            typedef size_t size_type;
            typedef _rb_tree_iterator<T, T*, T&> iterator;
            typedef _rb_tree_iterator<T, const T*, const T&> const_iterator;
            typedef _rb_tree_iterator self;
            typedef typename _rb_tree_node<T>::link_type link_type;

        // constructor
        _rb_tree_iterator() {}
        
        _rb_tree_iterator(link_type ptr)
        {
            node = ptr;
        }

        _rb_tree_iterator(const iterator& rhs)
        {
            node = rhs.node;
        }

        // operator
        reference operator*()
        {
            return static_cast<link_type>(node)->value;
        }

        pointer operator->()
        {
            return &(operator*());
        }

        self& operator++()
        {
            increment();
            return *this;
        }

        self operator++(int)
        {
            self temp = *this;
            operator++();
            return temp;
        }

        self& operator--()
        {
            decrement();
            return *this;
        }

        self operator--(int)
        {
            self temp = *this;
            operator--();
            return temp;
        }

    };

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = myAlloc>
    class rb_tree
    {
        private:
            typedef _rb_tree_node_base rb_tree_node_base;
            typedef rb_tree_node_base::base_ptr base_ptr;
            typedef _rb_tree_node<Value> rb_tree_node;
            typedef typename rb_tree_node::link_type link_type;
            typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
            typedef _rb_tree_color_type color_type;

        public:
            typedef Key key_type;
            typedef Value value_type;
            typedef value_type& reference;
            typedef const value_type& const_reference;
            typedef value_type* pointer;
            typedef const value_type* const_pointer;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef _rb_tree_iterator<value_type, pointer, reference> iterator;
            typedef _rb_tree_iterator<value_type, const_pointer, const_reference> const_iterator;
        
        public:
            // constructor
            rb_tree(const Compare& comp = Compare()) : node_count(0), key_comp(comp)
            {
                init();
            }

            template<class InputIterator>
            rb_tree(InputIterator first, InputIterator end, const Compare& comp = Compare()) : node_count(0), key_comp(comp)
            {
                init();
                for(; first != end; ++first)
                {
                    insert_unique(*first);
                }
            }

            rb_tree(const rb_tree& rhs) : node_count(0), key_comp(rhs.key_comp)
            {
                init();
                if(rhs.size() != 0)
                {
                    header->parent = _copy(rhs.root(), header);
                    header->left = rb_tree_node_base::minimum(header->parent);
                    header->right = rb_tree_node_base::maximum(header->parent);
                    node_count = rhs.size();
                }
            }

            ~rb_tree()
            {
                clear();
                put_node(header);
            }
            
            // modifier
            
            // for set and map
            pair<iterator, bool> insert_unique(const value_type& value);

            template<class InputIterator>
            void insert_unique(InputIterator first, InputIterator last)
            {
                for(; first != last; ++first)
                    insert_unique(*first);
            }

            iterator insert_unique(iterator position, const value_type& value);

            // for multiset and multimap
            iterator insert_equal(const value_type& value);

            template<class InputIterator>
            void insert_equal(InputIterator first, InputIterator last);

            iterator insert_equal(iterator position, const value_type& value);

            iterator erase(iterator position);

            size_type erase(const key_type& value)
            {
                pair<iterator, iterator> p = equal_range(value);
                difference_type n = distance(p.first, p.second);
                iterator first = p.first;
                iterator last = p.second;

                while(first != last)
                    erase(first++);
                return static_cast<size_type>(n);
            }

            void erase(iterator first, iterator last);

            void clear();

            Compare key_compare() const
            {
                return key_comp;
            }
        
            // iterator
            iterator begin()
            {
                return iterator(leftMost());
            }

            const_iterator begin() const
            {
                return const_iterator(leftMost());
            }

            const_iterator cbegin() const
            {
                return const_iterator(leftMost());
            }

            iterator end()
            {
                return iterator(header);
            }

            const_iterator end() const
            {
                return const_iterator(header);
            }

            const_iterator cend() const
            {
                return const_iterator(header);
            }

            // capacity
            size_type size() const
            {
                return node_count;
            }

            bool empty() const
            {
                return size() == 0;
            }

            size_type count(const key_type& key) const
            {
                return distance(lower_bound(key), upper_bound(key));
            }

            // verify
            size_type _black_count(base_ptr node, base_ptr root) const
            {
                if(node == NULL)
                    return 0;
                else
                {
                    size_t cn = node->color == _rb_tree_black? 1 : 0;
                    if(root == node)
                        return cn;
                    else
                        return cn + _black_count(node->parent, root);
                }
            }

            bool rb_tree_verify() const
            {
                if(node_count == 0 || begin() == end())
                    return node_count == 0 && begin() == end() && leftMost() == header && rightMost() == header;

                size_t len = _black_count(leftMost(), root());
                for(auto iter = cbegin(); iter != cend(); ++iter)
                {
                    link_type x = static_cast<link_type>(iter.node);
                    link_type L = left(x);
                    link_type R = right(x);

                    if(x->color == _rb_tree_red)
                        if((L && L->color == _rb_tree_red) || (R && R->color == _rb_tree_red))
                        {
                            return false;
                        }

                    if(L && key_comp(key(x), key(L)))
                    {
                        return false;
                    }
                    if(R && key_comp(key(R), key(x)))
                    {
                        return false;
                    }
                    if(L == NULL && R == NULL && _black_count(x, root()) != len)
                    {
                        return false;
                    }
                }

                if(leftMost() != rb_tree_node_base::minimum(root()))
                    return false;
                if(rightMost() != rb_tree_node_base::maximum(root()))
                    return false;
                return true;
            }


            // upper_bound and lower_bound
            iterator lower_bound(const key_type& value) const
            {
                link_type y = header;   // 最后一个不小于value的节点
                link_type x = root();

                while(x != NULL)
                {
                    if(!key_comp(key(x), value))
                    {
                        y = x;
                        x = static_cast<link_type>(x->left);
                    }
                    else
                        x = static_cast<link_type>(x->right);
                }

                return iterator(y);
            }

            iterator upper_bound(const key_type& value) const 
            {
                link_type y = header;   //最后一个大于value的节点
                link_type x = root();

                while(x != NULL)
                {
                    if(key_comp(value, key(x)))
                    {
                        y = x;
                        x = left(x);
                    }
                    else
                        x = right(x);
                }
                
                return iterator(y);
            }

            pair<iterator, iterator> equal_range(const key_type& value) const
            {
                return make_pair(lower_bound(value), upper_bound(value));
            }

            iterator find(const key_type& key_value);

            void swap(rb_tree &rhs)
            {
                using MiniStl::swap;

                swap(header, rhs.header);
            }

            rb_tree& operator=(const rb_tree& rhs)
            {
                clear();
                
                if(!rhs.empty())
                {
                    header->parent = _copy(rhs.root(), header);
                    header->left = rb_tree_node_base::minimum(root());
                    header->right = rb_tree_node_base::maximum(root());
                    node_count = rhs.node_count;
                }

                return *this;
            }

            bool operator==(const rb_tree& rhs) const
            {
                if(rhs.size() != size())
                    return false;

                const_iterator first1 = rhs.begin();
                const_iterator last1 = rhs.end();
                const_iterator first2 = begin();
                const_iterator last2 = end();

                for(; first1 != last1 && first2 != last2; ++first1, ++first2)
                    if(*first1 != *first2)
                        return false;
                return true;
            }

            bool operator!=(const rb_tree& rhs) const
            {
                return !(operator==(rhs));
            }

        private:
            // 真正的插入函数
            iterator _insert(base_ptr x, base_ptr p, const value_type& value);

            // 插入的重新平衡
            void _rb_tree_rebalance(base_ptr x, base_ptr& root);

            // 擦除平衡         
            base_ptr _rb_tree_rebalance_for_erase(base_ptr z, base_ptr& root, base_ptr& leftmost, base_ptr& rightmode);

            void _rb_tree_rotate_left(base_ptr x, base_ptr& root);

            void _rb_tree_rotate_right(base_ptr x, base_ptr& root);

            base_ptr _copy(link_type x, link_type parent);

            void clear_aux(base_ptr ptr);

        private:
            link_type header;
            size_type node_count;
            Compare key_comp;

        private:
            link_type& root() const
            {
                return  (link_type&)header->parent;
            }

            link_type& rightMost() const
            {
                return (link_type&)header->right;
            }

            link_type& leftMost() const
            {
                return (link_type&)header->left;
            }

            // 提取内部成员
            static link_type& left(link_type x) 
            {
                return (link_type&)x->left;
            }

            static link_type& right(link_type x)
            {
                return (link_type&)x->right;
            }

            static link_type& parent(link_type x)
            {
                return (link_type&)x->parent;
            }

            static reference value(link_type x)
            {
                return x->value;
            }

            static const key_type& key(link_type x)
            {
                return KeyOfValue()(value(x));
            }

            static color_type& color(link_type x)
            {
                return x->color;
            }

            // 同上
            static link_type& left(base_ptr x)
            {
                return (link_type&)x->left;
            }

            static link_type& right(base_ptr x)
            {
                return (link_type&)x->right;
            }

            static link_type& parent(base_ptr x)
            {
                return (link_type&)x->parent;
            }

            static reference value(base_ptr x)
            {
                return static_cast<link_type>(x)->value;
            }

            static const key_type& key(base_ptr x)
            {
                return KeyOfValue()(value(x));
            }

            static color_type& color(base_ptr x)
            {
                return x->color;
            }

            // 极大值与极小值
            static link_type minimum(link_type ptr)
            {
                return static_cast<link_type>(rb_tree_node_base::minimum(ptr));
            }

            static link_type maximum(link_type ptr)
            {
                return static_cast<link_type>(rb_tree_node_base::maximum(ptr));
            }

        private:
            link_type get_node()
            {
                return rb_tree_node_allocator::allocate();
            }

            void put_node(link_type ptr)
            {
                rb_tree_node_allocator::deallocate(ptr);
            }

            link_type create_node(const value_type& value)
            {
                link_type temp = get_node();
                construct(&(temp->value), value);
                temp->left = NULL;
                temp->right = NULL;
                temp->parent = NULL;
                return temp;
            }

            void destory_node(base_ptr ptr)
            {
                link_type _ptr = static_cast<link_type>(ptr);
                destory(&_ptr->value);
                put_node(_ptr);
            }

            link_type clone_node(link_type x)
            {
                link_type temp = create_node(x->value);
                temp.color = x->color;
                temp.left = NULL;
                temp.right = NULL;
                temp.parent = NULL;
            }

            void init()
            {
                header = get_node();
                header->parent = NULL;
                header->color = _rb_tree_red;
                header->left = static_cast<base_ptr>(header);
                header->right = static_cast<base_ptr>(header);
            }
    };

    
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& value)
    {
        link_type p = header;
        link_type x = root();
        bool comp = true;
        while(x != 0)
        {
            p = x;
            comp = key_comp(KeyOfValue()(value), key(x));
            x = comp? left(x) : right(x);
        }

        iterator j(p);
        // 如果最后结果是左节点
        if(comp)
        {
            if(j == begin())
            {
                return pair<iterator, bool>(_insert(x, p, value), true);
            }
            else
            {
                // 此时j为比父节点小的节点
                --j;
            }
        }
        
        // 如果大于前一个节点, 则可插入
        if(key_comp(key(j.node), KeyOfValue()(value)))
        {
            return pair<iterator, bool>(_insert(x, p, value), true);
        }

        // 返回与插入值相同的节点
        return pair<iterator, bool>(j, false);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const value_type& value)
    {   
        if(position.node == header->left) // 头节点
        {
            if(node_count != 0 && key_comp(KeyOfValue()(value), key(leftMost())))
                return _insert(position.node, position.node, value);
            else
                return (insert_unique(value)).first;
        }
        else if(position.node == header)    // 尾节点
        {
            if(node_count != 0 && key_comp(key(rightMost()), KeyOfValue()(value)))
                return _insert(NULL, rightMost(), value);           // 插入最大节点的右边
            else
                return (insert_unique(value)).first;
        }
        else
        {
            iterator before = position;
            --before;
            if(key_comp(key(before.node), KeyOfValue()(value)) && key_comp(KeyOfValue()(value), key(position.node)))
            {
                if(before.node->right == NULL)
                {
                    return _insert(NULL, before.node, value);
                }
                else
                {
                    return _insert(position.node, position.node, value);
                }
            }
            return (insert_unique(value)).first;
        }
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& value)
    {
        link_type p = header;
        link_type x = root();
        
        while(x != NULL)
        {
            p = x;
            x = key_comp(KeyOfValue()(value), key(x)) ? left(x) : right(x);
        }

        return _insert(x, p, value);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator position, const value_type& value)
    {
        if(position.node == header->left) // 首节点
        {
            if(node_count != 0 && key_comp(KeyOfValue()(value), key(leftMost())))
            {
                return _insert(position.node, position.node, value);
            }
            else
            {
                return insert_equal(value);
            }
        }
        else if(position.node == header) // 尾节点
        {
            if(node_count != 0 && !key_comp(KeyOfValue()(value), key(rightMost())))
            {
                return _insert(NULL, position.node, value);
            }
            else
            {
                return insert_equal(value);
            }
        }
        else
        {
            iterator j = position;
            --j;

            if(!key_comp(KeyOfValue()(value), key(j.node)) && !key_comp(key(position.node), KeyOfValue()(value)))
            {
                if(j.node->right == NULL)
                    return _insert(NULL, j.node, value);
                else
                    return _insert(position.node, position.node, value);
            }

            return insert_equal(value);
        }
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc> template<class InputIterator>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(InputIterator first, InputIterator last)
    {
        while(first != last)
            insert_equal(*(first++));
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
    {
        clear_aux(header->parent);
        header->parent = NULL;
        header->left = static_cast<base_ptr>(header);
        header->right = static_cast<base_ptr>(header);
        node_count = 0;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear_aux(base_ptr ptr)
    {
        if(ptr == NULL)
            return;
        
        if(ptr->left)
            clear_aux(ptr->left);
        if(ptr->right)
            clear_aux(ptr->right);
        destory_node(static_cast<link_type>(ptr));
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(link_type x, link_type parent)
    {   
        // 基准
        if(x == NULL)
            return x;

        // 创建x
        link_type newPtr = create_node(x->value);
        newPtr->color = x->color;
        // 记录parent节点
        newPtr->parent = parent;
        newPtr->left = _copy(static_cast<link_type>(x->left), newPtr);
        newPtr->right = _copy(static_cast<link_type>(x->right), newPtr);
        return newPtr;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(base_ptr x, base_ptr p, const value_type& value)
    {
        link_type _x = static_cast<link_type>(x);
        link_type _p = static_cast<link_type>(p);
        link_type temp;

        if(_p == header || _x != NULL || key_comp(KeyOfValue()(value), key(_p)))
        {
            temp = create_node(value);
            left(_p) = temp;
            // 如果parent是header节点, 需要调节最大节点为该节点
            if(_p == header)
            {
                rightMost() = temp;
                root() = temp;
            }
            // 如果parent是此时的最小节点, 则将最小节点调整为此节点
            if(_p == leftMost())
                leftMost() = temp;
        }
        else
        {
            temp = create_node(value);
            right(_p) = temp;
            // 如果parent是最大节点, 则将最大节点调整为此节点
            if(_p == rightMost())
                rightMost() = temp;
        }

        parent(temp) = _p;
        left(temp) = NULL;
        right(temp) = NULL;

        _rb_tree_rebalance(temp, header->parent);
        ++node_count;
        return iterator(temp);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rebalance(base_ptr x, base_ptr& root)
    {
        color(x) = _rb_tree_red;    //新节点应该为红色
        while(x != root && x->parent->color == _rb_tree_red)    //如果x不为头节点, 并且父亲为红色
        {
            if(x->parent->parent->left == x->parent)    // 父亲为祖父的左儿子
            {
                base_ptr s = x->parent->parent->right;
                if(s && s->color == _rb_tree_red)       // 存在伯父并且伯父也为红色, 此为第三种情况
                {
                    s->color = _rb_tree_black;          // 将伯父和父亲都改为黑色
                    x->parent->color = _rb_tree_black;  
                    x->parent->parent->color = _rb_tree_red; // 祖父改为红色
                    x = x->parent->parent;  //将x改为祖父，继续向上检查
                }
                else
                {
                    if(x == x->parent->right)   //第二种情况, 插入点为父亲的右节点
                    {
                        x = x->parent;
                        _rb_tree_rotate_left(x, root);  // 此时变成第一种情况
                    }
                    // 第一种情况
                    x->parent->color = _rb_tree_black;  // 将父节点改为黑色
                    x->parent->parent->color = _rb_tree_red; // 将祖父改为红色
                    _rb_tree_rotate_right(x->parent->parent, root);
                }
            }
            else    //父亲为祖父的右儿子, 情况与前面一样, 但是方法相反
            {
                base_ptr s = x->parent->parent->left;
                if(s && s->color == _rb_tree_red)
                {
                    s->color = _rb_tree_black;
                    x->parent->color = _rb_tree_black;
                    x->parent->parent->color = _rb_tree_red;
                    x = x->parent->parent;
                }
                else
                {
                    if(x == x->parent->left)
                    {
                        x = x->parent;
                        _rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = _rb_tree_black;
                    x->parent->parent->color = _rb_tree_red;
                    _rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = _rb_tree_black;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rebalance_for_erase(base_ptr z, base_ptr& root, base_ptr& leftmost, base_ptr& rightmost)
    {
        base_ptr y = z;     // y为擦除的节点, 或者是插入树中的节点
        base_ptr x = NULL;  // 代替y的节点
        base_ptr x_parent = NULL; // 记忆x的parent节点

        if(y->left == NULL) // z只有右节点的情况
            x = y->right;   // 记住y的替代节点
        else
        {
            if(y->right == NULL) // z只有左节点的情况
                x = y->left;    // 记住y的替代节点
            else                 // z有左右节点的情况
            {
                y = y->right;   // 找出z的继承节点
                while(y->left != NULL)
                    y = y->left;
                x = y->right;   // 记住y的替代节点
            }
        }

        if(y != z)      // z有左右节点的情况
        {
            z->left->parent = y; // 链接z的左节点
            y->left = z->left;
            if(y != z->right)   // 如果y不是z的右节点
            {
                x_parent = y->parent;   // 记住x的parent
                if(x)   // 如果x存在，则更改x的parent
                {
                    x->parent = y->parent;
                }
                y->parent->left = x;    // 替换y的位置
                y->right = z->right;    // 链接z的右节点
                z->right->parent = y;   // 更换z的右节点的parent
            }
            else    // y是z的右节点, 此时不需要重新链接x
                x_parent = y;
            if(root == z)               // 正式更换z的parent
                root = y;
            else if(z->parent->left == z)
                z->parent->left = y;
            else
                z->parent->right = y;
            y->parent = z->parent;
            using std::swap;
            swap(y->color, z->color);
            y = z;
        }
        else    // z至多有一个孩子, 所以此时z可能是最小或者最大的节点，需要检查
        {
            x_parent = y->parent;   // 保存x的parent
            if(x)   // 如果x存在，更换x的parent
            {
                x->parent = y->parent;
            }
            if(root == z)   // 更换z的parent的孩子
                root = x;
            else
            {
                if(z->parent->left == z)
                    z->parent->left = x;
                else
                    z->parent->right = x;
            }
            if(leftmost == z)
            {
                if(z->right == NULL)
                    leftmost = z->parent;
                else
                    leftmost = rb_tree_node_base::minimum(x);
            }
            if(rightmost == z)
            {
                if(z->left == NULL)
                    rightmost = z->parent;
                else
                {
                    rightmost = rb_tree_node_base::maximum(x);
                }
            }
        }

        // 开始修复红黑树, 如果删除的节点为红色或者插入树中的节点为红色, 则不用改变
        if(y->color != _rb_tree_red)
        {
            while(x != root && (x == NULL || x->color == _rb_tree_black))
                if(x == x_parent->left) // x为左节点
                {
                    base_ptr w = x_parent->right;       // x的兄弟
                    if(w->color == _rb_tree_red)        // case 1
                    {
                        w->color = _rb_tree_black;
                        x_parent->color = _rb_tree_red;
                        _rb_tree_rotate_left(x_parent, root);
                        w = x_parent->right;
                    }
                    if((w->left == NULL || w->left->color == _rb_tree_black) && 
                        (w->right == NULL || w->right->color == _rb_tree_black))    // case 2
                    {
                        w->color = _rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if(w->right == NULL || w->right->color == _rb_tree_black) // case3
                        {
                            if(w->left)
                                w->left->color = _rb_tree_black;
                            w->color = _rb_tree_red;
                            _rb_tree_rotate_right(w, root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;     // case 4
                        x_parent->color = _rb_tree_black;
                        if(w->right)
                            w->right->color = _rb_tree_black;
                        _rb_tree_rotate_left(x_parent, root);
                        break;
                    }
                }
                else
                {
                    base_ptr w = x_parent->left;        // x的兄弟
                    if(w->color == _rb_tree_red)        // case 1
                    {
                        w->color = _rb_tree_black;
                        x_parent->color = _rb_tree_red;
                        _rb_tree_rotate_right(x_parent, root);
                        w = x_parent->left;
                    }
                    if((w->right == NULL || w->right->color == _rb_tree_black) && 
                        (w->left == NULL || w->left->color == _rb_tree_black))  // case 2
                    {
                        w->color = _rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if(w->left == NULL || w->left->color == _rb_tree_black) // case3
                        {
                            if(w->right)
                                w->right->color = _rb_tree_black;
                            w->color = _rb_tree_red;
                            _rb_tree_rotate_left(w, root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;     // case 4
                        x_parent->color = _rb_tree_black;
                        if(w->left)
                            w->left->color = _rb_tree_black;
                        _rb_tree_rotate_right(x_parent, root);
                        break;
                    }
                }
            if(x) x->color = _rb_tree_black;
        }
        return y;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
    {
        base_ptr ptr = _rb_tree_rebalance_for_erase((position++).node, header->parent, header->left, header->right);

        destory(ptr);
        --node_count;
        return position;
    }
    
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& key_value)
    {
        link_type y = header;   // 最后一个不小于value的节点
        link_type x = root();

        while(x != NULL)
        {
            if(!key_comp(key(x), key_value))
            {
                y = x;
                x = left(x);
            }
            else
            {
                x = right(x);
            }
        }

        iterator j(y);

        return j == end() || key_comp(key_value, key(y)) ? end() : j;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last)
    {
        if(first == begin() && last == end())
            clear();
        else
        {
            while(first != last)
                erase(first++);
        }
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rotate_right(base_ptr x, base_ptr& root)
    {
        base_ptr son = x->left;
        x->left = son->right;
        if(son->right)
        {
            x->left->parent = x;
        }
        son->parent = x->parent;
        if(x == root)
        {
            root = son;
        }
        else if(x->parent->left == x)
        {
            x->parent->left = son;
        }
        else
        {
            x->parent->right = son;
        }
        x->parent = son;
        son->right = x;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rotate_left(base_ptr x, base_ptr& root)
    {
        base_ptr son = x->right;
        x->right = son->left;
        if(son->left)
        {
            x->right->parent = x;
        }
        son->parent = x->parent;
        if(x == root)
        {
            root = son;
        }
        else if(x->parent->left == x)
        {
            x->parent->left = son;
        }
        else
        {
            x->parent->right = son;
        }
        son->left = x;
        x->parent = son;
    }
}


#endif
