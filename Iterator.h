#ifndef __STL_ITERATOR_H_
#define __STL_ITERATOR_H_

#include <cstddef>
#include <iostream>
namespace MiniStl
{
    class input_iterator_tag {};
    class output_iterator_tag {};
    class forward_iterator_tag : public input_iterator_tag {};
    class bidirectional_iterator_tag : public forward_iterator_tag {};
    class random_iterator_tag : public bidirectional_iterator_tag {};

    // 迭代器萃取器
    template <class Iterator>
    struct _iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    // 对指针偏例化
    template <class T>
    struct _iterator_traits<T *>
    {
        typedef random_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    template <class T>
    struct _iterator_traits<const T*>
    {
        typedef random_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
    };

    // 迭代器类型
    template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    // 萃取迭代器的类型
    template <class Iterator>
    inline typename _iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
    {
        typedef typename _iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    // 萃取迭代器指向对象的类型
    template <class Iterator>
    inline typename _iterator_traits<Iterator>::value_type* value_type(const Iterator &)
    {
        typedef typename _iterator_traits<Iterator>::value_type value_type;
        return static_cast<value_type *>(NULL);
    }

    // 萃取迭代器距离的类型
    template <class Iterator>
    inline typename _iterator_traits<Iterator>::difference_type* difference_type(const Iterator &)
    {
        typedef typename _iterator_traits<Iterator>::difference_type difference_type;
        return static_cast<difference_type*>(NULL);
    }

    // advance函数
    template<class InputIterator, class Size>
    inline void _advance(InputIterator &first, Size n, input_iterator_tag)
    {
        for(; n != 0; --n)
            ++first;
    }

    template<class BidirectionalIterator, class Size>
    inline void _advance(BidirectionalIterator &first, Size n, bidirectional_iterator_tag)
    {
        if(n > 0)
        {
            for(; n != 0; --n)
                ++first;
        }
        else
        {
            for(; n != 0; ++n)
                --first;
        }
    }

    template<class RandomIterator, class Size>
    inline void _advance(RandomIterator &first, Size n, random_iterator_tag)
    {
        first += n;
    }

    template<class InputIterator, class Size>
    inline void advance(InputIterator &first, Size n)
    {
        _advance(first, n, iterator_category(first));
    }
    // advance函数结束

    // distance函数
    template<class InputIterator>
    inline typename _iterator_traits<InputIterator>::difference_type _distance(InputIterator first, InputIterator end, input_iterator_tag)
    {
        typedef typename _iterator_traits<InputIterator>::difference_type Distance;

        Distance n = 0;
        while(first != end)
        {
            ++n;
            ++first;
        }

        return n;
    }

    template<class RandomIterator>
    inline typename _iterator_traits<RandomIterator>::difference_type _distance(RandomIterator first, RandomIterator end, random_iterator_tag)
    {
        return end - first;
    }

    template<class InputIterator>
    inline typename _iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator end)
    {
        return _distance(first, end, iterator_category(first));
    }
    // distance函数结束



    // 迭代器配接器
    template<class Container>
    class back_insert_iterator
    {
        private:
            Container* container;
        public:
            typedef output_iterator_tag iterator_category;
            typedef void value_type;
            typedef void pointer;
            typedef void difference_type;
            typedef void reference;

            explicit back_insert_iterator(Container& c) : container(&c) {}

            back_insert_iterator& operator=(const typename Container::value_type& value)
            {
                container->push_back(value);
                return *this;
            }

            // 对以下操作都不进行操作, 只是单纯返回自己
            back_insert_iterator& operator++()
            {
                return *this;
            }

            back_insert_iterator& operator++(int)
            {
                return *this;
            }

            back_insert_iterator& operator*()
            {
                return *this;
            }
    };

    // convenience function template that construct a back_insert_iterator
    template<class Container>
    inline back_insert_iterator<Container> back_inserter(Container& c)
    {
        return back_insert_iterator<Container>(c);
    }

    template<class Container>
    class front_insert_iterator
    {
        private:
            Container* container;

        public:
            typedef output_iterator_tag iterator_category;
            typedef void value_type;
            typedef void pointer;
            typedef void reference;
            typedef void difference_type;

            explicit front_insert_iterator(Container& c) : container(&c) {}
            
            front_insert_iterator& operator=(const typename Container::value_type& value)
            {
                container->push_front(value);
                return *this;
            }

            front_insert_iterator& operator++()
            {
                return *this;
            }

            front_insert_iterator& operator++(int)
            {
                return *this;
            }

            front_insert_iterator& operator*()
            {
                return *this;
            }
    };

    // convenience function template that construct a front_inserter_iterator
    template<class Container>
    inline front_insert_iterator<Container> front_inserter(Container& c)
    {
        return front_insert_iterator<Container>(c);
    }

    template<class Container>
    class insert_iterator
    {
        private:
            Container *container;
            typename Container::iterator iter;

        public:
            explicit insert_iterator(Container &c, const typename Container::iterator& it) : container(&c), iter(it) {}

            insert_iterator& operator=(const typename Container::value_type& value)
            {
                iter = container->insert(iter, value);
                ++iter;
                return *this;
            }

            insert_iterator& operator++()
            {
                return *this;
            }

            insert_iterator& operator++(int)
            {
                return *this;
            }

            insert_iterator& operator*()
            {
                return *this;
            }
    };

    // convenience function template that construct a inserter_iterator, 参考条款43
    template<class Container>
    inline insert_iterator<Container> inserter(Container& c, typename Container::iterator iter)
    {
        return insert_iterator<Container>(c, iter);
    }

    template<class Iterator>
    class reverser_iterator
    {
        private:
            Iterator iter;

        public:
            typedef typename _iterator_traits<Iterator>::value_type value_type;
            typedef typename _iterator_traits<Iterator>::pointer pointer;
            typedef typename _iterator_traits<Iterator>::reference reference;
            typedef typename _iterator_traits<Iterator>::difference_type difference_type;
            typedef typename _iterator_traits<Iterator>::iterator_category iterator_category;

            explicit reverser_iterator(const Iterator& iterator) : iter(iterator) {}

            reference operator*()
            {
                Iterator temp = iter;
                return *(--temp);
            }

            reverser_iterator& operator++()
            {
                --iter;
                return *this;
            }

            reverser_iterator& operator--()
            {
                ++iter;
                return *this;
            }

            reverser_iterator operator++(int)
            {
                reverser_iterator temp = *this;
                --iter;
                return temp;
            }

            reverser_iterator operator--(int)
            {
                reverser_iterator temp = *this;
                ++iter;
                return temp;
            }

            pointer operator->()
            {
                return &(operator*());
            }

            // 参考条款28, 注意转换细节
            Iterator base()
            {
                return iter;
            }

            reverser_iterator operator+(difference_type n) const
            {
                return reverser_iterator(iter - n);
            }

            reverser_iterator operator-(difference_type n) const
            {
                return reverser_iterator(iter + n);
            }

            reverser_iterator& operator+=(difference_type n)
            {
                iter = iter - n;
                return *this;
            }
            
            reverser_iterator& operator-=(difference_type n)
            {
                iter = iter + n;
                return *this;
            }

            reference operator[](difference_type n)
            {
                return *(*this + n);
            }

            bool operator==(const reverser_iterator& rhs) const
            {
                return iter == rhs.iter;
            }

            bool operator!=(const reverser_iterator& rhs) const
            {
                return !(operator==(rhs));
            }
    };

    template<class Iterator>
    inline bool operator==(const reverser_iterator<Iterator>& lhs, const reverser_iterator<Iterator>& rhs)
    {
        return lhs.operator==(rhs);
    }

    template<class Iterator>
    inline bool operator!=(const reverser_iterator<Iterator>& lhs, const reverser_iterator<Iterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template<class T, class Distance = ptrdiff_t>
    class istream_iterator;

    template<class T, class Distance>
    inline bool operator==(const istream_iterator<T, Distance>& lhs, const istream_iterator<T, Distance>& rhs);

    template<class T, class Distance>
    class istream_iterator
    {
        friend bool operator==<>(const istream_iterator& lhs, const istream_iterator& rhs);

        private:
            std::istream* stream;
            T value;
            bool end_marker;
            void read()
            {
                end_marker = *stream? false : true;
                if(!end_marker)
                    *stream >> value;
                end_marker = *stream? false : true;
            }

        public:
            typedef input_iterator_tag iterator_category;
            typedef T value_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef Distance difference_type;

        public:
            istream_iterator() : stream(&std::cin), end_marker(true) {}

            istream_iterator(std::istream& s) : stream(&s)
            {
                read();
            }

            reference operator*()
            {
                return value;
            }

            pointer operator->()
            {
                return &(operator*());
            }

            istream_iterator& operator++()
            {
                read();
                return *this;
            }

            istream_iterator operator++(int)
            {
                istream_iterator temp = *this;
                read();
                return temp;
            }
    };

    template<class T>
    class ostream_iterator
    {
        private:
            std::ostream *stream;
            const char* str;

        public:
            typedef output_iterator_tag iterator_category;
            typedef void value_type;
            typedef void difference_type;
            typedef void pointer;
            typedef void reference;

            ostream_iterator(std::ostream &st, const char* s) : stream(&st), str(s) {}
            
            explicit ostream_iterator(std::ostream &st) : stream(&st), str(NULL) {}

            ostream_iterator& operator=(const T& value)
            {
                *stream << value;
                if(str)
                    *stream << str;
                return *this;
            }

            ostream_iterator& operator++()
            {
                return *this;
            }

            ostream_iterator& operator++(int)
            {
                return *this;
            }

            ostream_iterator& operator*()
            {
                return *this;
            }
    };

    template<class T, class Distance>
    inline bool operator==(const istream_iterator<T, Distance>& lhs, const istream_iterator<T, Distance>& rhs)
    {
        return (lhs.stream == rhs.stream && lhs.end_marker == rhs.end_marker) || (rhs.end_marker == false && lhs.end_marker == false);
    }

    template<class T, class Distance>
    inline bool operator!=(const istream_iterator<T, Distance>& lhs, const istream_iterator<T, Distance>& rhs)
    {
        return !(lhs == rhs);
    }
}
#endif
