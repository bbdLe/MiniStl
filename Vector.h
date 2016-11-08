#ifndef __MINISTL_VECTOR_H
#define __MINISTL_VECTOR_H

#include<stdexcept> // out_of_range
#include"Utility.h"
#include"Allocator.h"
#include"Uninitialized.h"
#include"TypeTraits.h"
#include"Iterator.h"

namespace MiniStl
{
    template<class T, class Alloc = myAlloc>
    class vector
    {
        public:
            typedef T value_type;
            typedef value_type* pointer;
            typedef const value_type* const_pointer;
            typedef value_type& reference;
            typedef const value_type& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            // 迭代器, 直接用指针当迭代器
            typedef value_type* iterator;
            typedef const value_type* const_iterator;
            typedef reverser_iterator<iterator> reverse_iterator;
            typedef reverser_iterator<const_iterator> const_reverse_iterator;

        public:
            //构造函数
            vector() : start(NULL), finish(NULL), end_of_storage(NULL) {}

            vector(size_type n, const value_type &value) : start(NULL), finish(NULL), end_of_storage(NULL)
            {
                fill_initialize(n, value);
            }

            vector(int n, const value_type &value) : start(NULL), finish(NULL), end_of_storage(NULL)
            {
                fill_initialize(n, value);
            }

            vector(long n, const value_type &value) : start(NULL), finish(NULL), end_of_storage(NULL)
            {
                fill_initialize(n, value);
            }

            explicit vector(size_type n) : start(NULL), finish(NULL), end_of_storage(NULL)
            {
                fill_initialize(n, T());
            }

            template<class InputIterator>
            vector(InputIterator first, InputIterator end)
            {
                range_initialize(first, end, iterator_category(first));
            }

            vector(const vector<T, Alloc> &other)
            {
                start = allocate_and_copy(other.end() - other.begin(), other.begin(), other.end());
                finish = start + (other.end() - other.begin());
                end_of_storage = finish;
            }

            //析构函数
            ~vector()
            {
                destory(start, finish);
                deallocate();
            }
            
            // operator assignment, copy and swap
            vector &operator=(const vector &rhs)
            {
                vector temp(rhs);
                swap(temp);
                return *this;
            }

        public:
            //Capacity
            size_type capacity() const
            {
                return end_of_storage - start;
            }

            size_type size() const
            {
                return finish - start;
            }

            // 比直接调用size() == 0快, 参考条款四
            bool empty() const
            {
                return start == finish;
            }

            void reserve(size_type n)
            {
                if(capacity() < n)
                {
                    const size_type oldSize = size();
                    iterator temp = allocate_and_copy(n, start, finish);
                    destory(start, finish);
                    deallocate();
                    start = temp;
                    finish = start + oldSize;
                    end_of_storage = start + n;
                }
            }

            void resize(size_type newSize)
            {
                resize(newSize, T());
            }

            void resize(size_type newSize, const T& value)
            {
                if(newSize < size())
                {
                    erase(begin() + newSize, finish);
                }
                else
                {
                    insert(end(), newSize - size(), value);
                }
            }

            // 参考条款17, 去除多余容量
            void shrink_to_fit()
            {
                vector other(begin(), end());
                swap(other);
            }

            // Element access

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
                return *(end() - 1);
            }

            const_reference back() const
            {
                return *(end() - 1);
            }
            
            reference operator[](size_type n)
            {
                return *(start + n);
            }

            const_reference operator[](size_type n) const
            {
                return *(start + n);
            }

            reference at(size_type n)
            {
                if(n >= size())
                    throw std::out_of_range("Out of range");
                return operator[](n);
            }

            const_reference at(size_type n) const
            {
                if(n >= size())
                    throw std::out_of_range("Out of range");
                return operator[](n);
            }

            pointer data() noexcept
            {
                return start;
            }

            const_pointer data() const noexcept
            {
                return start;
            }

            // Iterators

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

            reverse_iterator rbegin()
            {
                return reverse_iterator(end());
            }

            const_reverse_iterator rbegin() const
            {
                return const_reverse_iterator(end());
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

            reverse_iterator rend()
            {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rend() const
            {
                return const_reverse_iterator(begin());
            }
        
        private:
            typedef simple_alloc<T, myAlloc> data_allocator;

            iterator start;
            iterator finish;
            iterator end_of_storage;

        private:
            void deallocate();

            void fill_initialize(size_type n, const value_type &value);
            
            template<class ForwardIterator>
            void range_initialize(ForwardIterator first, ForwardIterator end, forward_iterator_tag);

            template<class InputIterator>
            void range_initialize(InputIterator first, InputIterator end, output_iterator_tag);

            iterator allocate_and_fill(size_type n, const value_type &value);

            template<class Size, class ForwardIterator>
            iterator allocate_and_copy(Size n, ForwardIterator first, ForwardIterator end);
        
        public:
            // Modifiers
            void push_back(const T& value)
            {
                if(finish != end_of_storage)
                {
                    construct(finish, value);
                    ++finish;
                }
                else
                {   
                    insert_aux(end(), value);
                }
            }

            void pop_back()
            {
                --finish;
                destory(finish);
            }

            template<class InputIterator>
            void assign(InputIterator first, InputIterator end)
            {
                destory(start, finish);
                finish = start;
                range_insert(start, first, end, iterator_category(first));
            }

            void assign(size_type n, const value_type &value)
            {
                destory(start, finish);
                if(n > size())
                {
                    deallocate();
                    start = allocate_and_fill(n, value);
                    finish = start + n;
                    end_of_storage = finish;
                }
                else
                {
                    uninitialized_fill_n(start, n, value);
                    finish = start + n;
                }
            }

            iterator insert(iterator position, const T& value)
            {
                if(position == end())
                {
                    push_back(value);
                    return end();
                }
                else
                {
                    size_type n = position - begin();
                    insert_aux(position, value);
                    return begin() + n;
                }
            }
            
            template<class InputIterator>
            void insert(iterator position, InputIterator first, InputIterator end)
            {
                range_insert(position, first, end, iterator_category(first));
            }

            void insert(iterator position, size_type n, const T& value);

            iterator erase(iterator first, iterator end)
            {
                size_type n = end - first;
                iterator i = copy(end, finish, first);
                destory(i, finish);
                finish -= n;
                return first;
            }

            iterator erase(iterator position)
            {
                if(position != finish - 1)
                    copy(position + 1, finish, position);
                --finish;
                destory(finish);
                return position;
            }

            void clear()
            {
                erase(begin(), end());
            }

            void swap(vector &rhs)
            {
                using MiniStl::swap;
                swap(start, rhs.start);
                swap(finish, rhs.finish);
                swap(end_of_storage, rhs.end_of_storage);
            }

            bool operator==(const vector& rhs) const
            {
                if(size() != rhs.size())
                    return false;

                const_iterator first1 = begin();
                const_iterator end1 = end();
                const_iterator first2 = rhs.begin();
                const_iterator end2 = rhs.end();
                for(; first1 != end1 && first2 != end2; ++first1, ++first2)
                {
                    if(*first1 != *first2)
                        return false;
                }

                return true;
            }

            bool operator!=(const vector& rhs) const
            {
                return !(operator==(rhs));
            }

        private:
            //内部使用函数
            void insert_aux(iterator position, const T& value);

            template<class ForwardIterator>
            void range_insert(iterator position, ForwardIterator first, ForwardIterator end, forward_iterator_tag);

            template<class InputIterator>
            void range_insert(iterator position, InputIterator first, InputIterator end, input_iterator_tag);
    };

    template<class T, class Alloc>
    void vector<T, Alloc>::deallocate()
    {
        data_allocator::deallocate(start, end_of_storage - start);
    }
    
    template<class T, class Alloc>
    void vector<T, Alloc>::fill_initialize(size_type n, const value_type &value)
    {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    
    // 对forward_iterator_tag优化, 能够一次性分配足够的空间!
    template<class T, class Alloc> template<class ForwardIterator>
    void vector<T, Alloc>::range_initialize(ForwardIterator first, ForwardIterator end, forward_iterator_tag)
    {
        typedef typename _iterator_traits<ForwardIterator>::difference_type difference_type;
        difference_type n = distance(first, end);
        start = allocate_and_copy(n, first, end);
        finish = start + n;
        end_of_storage = finish;
    }

    
    // output_iterator_tag 无法用减法导致无法一次性配置足够空间
    template<class T, class Alloc> template<class InputIterator>
    void vector<T, Alloc>::range_initialize(InputIterator first, InputIterator end, output_iterator_tag)
    {
        while(first != end)
        {
            push_back(*first);
            ++first;
        }
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(size_type n, const value_type &value)
    {
        iterator result = data_allocator::allocate(n);
        //Fixme
        uninitialized_fill_n(result, n, value);
        return result;
    }

    template<class T, class Alloc> template<class Size, class ForwardIterator>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_copy(Size n, ForwardIterator first, ForwardIterator end)
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_copy(first, end, result);
        return result;
    }
    
    template<class T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const T& value)
    {
        if(n != 0)
        {
            if(n <= end_of_storage - finish)    // 空间足够
            {
                size_type elementsLeft = finish - position;
                iterator oldFinish = finish;
                // 后面剩下的元素比插入的元素多
                if(elementsLeft > n)
                {
                    finish = uninitialized_copy(oldFinish - n, oldFinish, finish);  // 复制后面n个元素
                    copy_backward(position, oldFinish - n, oldFinish);              // 复制后面剩下的元素
                    uninitialized_fill_n(position, n, value);                       // fill新的元素
                }
                else
                {
                    finish = uninitialized_fill_n(finish, n - elementsLeft, value); // 复制多出来的元素
                    finish = uninitialized_copy(position, oldFinish, finish);       // 复制后面的元素
                    fill(position, oldFinish, value);                               // fill新的元素
                }
            }
            else        // 空间不足
            {
                const size_type oldSize = size();
                const size_type newSize = oldSize + (oldSize > n? oldSize : n);
                iterator newStart = data_allocator::allocate(newSize);
                iterator newFinish = newStart;
                newFinish = uninitialized_copy(start, position, newFinish);         // 复制前边的元素   
                newFinish = uninitialized_fill_n(newFinish, n, value);              // 复制n个新元素
                newFinish = uninitialized_copy(position, finish, newFinish);        // 复制后面的元素
                destory(start, finish);
                deallocate();
                start = newStart;
                finish = newFinish;
                end_of_storage = start + newSize;
            }
        }
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T& value)
    {
        if(finish != end_of_storage)
        {
            construct(finish, back());
            ++finish;
            copy_backward(position, finish - 2, finish - 1);
            *position = value;
        }
        else
        {
            const size_type positionIndex = position - start;
            const size_type oldSize = size();
            size_type newSize = (oldSize == 0? 2 : 2 * oldSize);
            iterator newStart = allocate_and_copy(newSize, start, position);
            iterator newFinish = newStart + positionIndex;
            construct(newFinish, value);
            ++newFinish;
            newFinish = uninitialized_copy(position, finish, newFinish);
            destory(start, finish);
            deallocate();
            start = newStart;
            finish = newFinish;
            end_of_storage = start + newSize;
        }
    }

    // 对ForwardIterator优化
    template<class T, class Alloc> template<class ForwardIterator>
    void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first, ForwardIterator end, forward_iterator_tag)
    {
        if(first != end)
        {
            size_type n = distance(first, end);
            if(n <= end_of_storage - finish)
            {
                const size_type elementsLeft = finish - position;
                iterator oldFinish = finish;
                //位置后的对象数量比插入的多
                if(elementsLeft > n)
                {
                    finish = uninitialized_copy(oldFinish - n, oldFinish, finish);
                    copy_backward(position, oldFinish - n, oldFinish);
                    copy(first, end, position);
                }
                //位置后的对象数量比插入的少
                else
                {
                    finish = uninitialized_copy(first + elementsLeft, end, finish);
                    finish = uninitialized_copy(position, oldFinish, finish);
                    copy(first, first + elementsLeft, position);
                }
            }
            // 重新分配内容，没有处理异常情况
            else
            {
                const size_type oldSize = size();
                const size_type newSize = oldSize + (n > oldSize? n : oldSize);
                iterator newStart = data_allocator::allocate(newSize);
                iterator newFinish = newStart;
                newFinish = uninitialized_copy(start, position, newFinish);
                newFinish = uninitialized_copy(first, end, newFinish);
                newFinish = uninitialized_copy(position, finish, newFinish);
                destory(start, finish);
                deallocate();
                start = newStart;
                finish = newFinish;
                end_of_storage = start + newSize;
            }
        }
    }

    // 针对iterator_category为input_iterator_tag的情况
    template<class T, class Alloc> template<class InputIterator>
    void vector<T, Alloc>::range_insert(iterator position, InputIterator first, InputIterator end, input_iterator_tag)
    {
        while(first != end)
        {
            position = insert(position, *first);
            ++position;
            ++first;
        }
    }

    template<class T, class Alloc>
    void swap(vector<T, Alloc> &lhs, vector<T, Alloc> &rhs)
    {
        lhs.swap(rhs);
    }
    
    template<class T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
