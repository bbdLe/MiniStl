#ifndef __MINISTL_BASESTRING_H
#define __MINISTL_BASESTRING_H
#include"Straits.h"
#include"Alloc.h"
#include<cstddef>

namespace MiniStl
{
    template<class charT, class traits = string_traits<charT>, class Alloc = myAlloc>
    class base_string
    {
        public:
            typedef charT value_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef const value_type* const_pointer;
            typedef const value_type& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef pointer iterator;
            typedef const_pointer const_iterator;

        private:
            typedef Alloc allocator;        // 不使用simple_alloc包裹
            pointer dat;

            struct __rep
            {
                size_t length;
                size_t capacity;
                size_t reference_count;
                bool selfish;
                
                void release()
                {
                    if(--reference_count == 0)
                        delete this;
                }

                pointer data()
                {
                    return reinterpret_cast<pointer>(this + 1);
                }

                reference operator[](size_type n)
                {
                    return data()[n];
                }

                static void* operator new(size_t size, size_t extra)
                {
                    return allocator::allocate(size + sizeof(value_type) * extra);
                }

                static void operator delete(void *ptr)
                {
                    allocator::deallocate(ptr, sizeof(__rep) + reinterpret_cast<__rep*>(ptr)->capacity * sizeof(value_type));
                }

                // 取16的倍数
                static size_t frob_size(size_t s)
                {
                    size_t i = 16;
                    while(i < s)
                        i *= 2;
                    return i;
                }

                static __rep* create(size_t extra)
                {
                    // 1 for c_str
                    extra = frob_size(extra + 1);
                    __rep *ptr = new (extra) __rep;
                    ptr->capacity = extra;
                    ptr->reference_count = 1;
                    ptr->selfish = false;
                    return ptr;
                }

                // 不需要2倍的空间
                static bool excess_slop(size_t s, size_t r)
                {
                    return 2 * (s <= 16? 16 : s) < r;
                }

                pointer clone()
                {
                    __rep *p = __rep::create(length);
                    p->copy(0, data(), length);
                    p->length = length;
                    return p->data();
                }
                
                void copy(size_t pos, const_pointer str, size_t n)
                {
                    if(n)
                        traits::copy(data() + pos, str, n);   
                }

                pointer grab()
                {
                    if(selfish)
                        return clone();
                    else
                    {
                        ++reference_count;
                        return data();
                    }
                }

                void move(size_t pos, const_pointer str, size_t n)
                {
                    if(n)
                        traits::move(data() + pos, str, n);
                }

                void set(size_t pos, value_type c, size_t n)
                {
                    if(n)
                        traits::set(data() + pos, c, n);
                }
            };

        public:
            // constructor
            template<class ForwardIterator>
            base_string(ForwardIterator first, ForwardIterator last) : dat(nil_rep.grab())
            {
                assign(first, last);
            }

            base_string(const_pointer str) : dat(nil_rep.grab())
            {
                assign(str);
            }

            base_string(const base_string& rhs) : dat(rhs.rep()->grab()) {}

            // destructor
            ~base_string()
            {
                rep()->release();
            }

            // modifier
            void clear()
            {
                erase(begin(), end());
            }

            base_string& insert(size_type pos, const base_string& str)
            {
                return replace(pos, 0, str);
            }

            base_string& insert(size_type pos, const_pointer str)
            {
                return replace(pos, 0, str);
            }

            iterator erase(iterator first, iterator last)
            {
                replace(first, last, static_cast<pointer>(NULL), static_cast<pointer>(NULL));
                return first;
            }

            base_string& erase(size_type pos, size_type len)
            {
                return replace(pos, len, "");
            }

            template<class ForwardIterator>
            base_string& assign(ForwardIterator first, ForwardIterator last)
            {
                return replace(begin(), end(), first, last);
            }

            base_string& assign(const_pointer str)
            {
                return replace(0, length(), str);
            }

            base_string& append(const base_string& other)
            {
                return replace(length(), 0, other);
            }

            template<class ForwardIterator>
            base_string& replace(iterator first1, iterator last1, ForwardIterator first2, ForwardIterator last2)
            {
                const size_type len = length();
                size_type pos = first1 - begin();
                size_type len1 = last1 - first1;
                size_type len2 = last2 - first2;

                if(len1 > len - pos)
                    len1 = len - pos;
                size_type new_size = len - len1 + len2;
                if(check_realloc(new_size))
                {
                    __rep* new_ptr = __rep::create(new_size);
                    new_ptr->copy(0, data(), pos);
                    new_ptr->copy(pos + len2, data() + pos + len1, len - pos - len1);
                    for(; first2 != last2; ++first2, ++pos)
                        traits::assign((*new_ptr)[pos], *first2);
                    repup(new_ptr);
                }
                else
                {
                    rep()->move(pos + len2, data() + pos + len1, len - pos - len1);
                    for(; first2 != last2; ++first2, ++pos)
                        traits::assign((*rep())[pos], *first2);
                }
                rep()->length = new_size;
                return *this;
            }

           base_string& replace(size_type pos, size_type len1, const_pointer str)
           {
                selfish();
                const size_type len = length();
                size_type len2 = traits::length(str);

                if(len1 > len - pos)
                    len1 = len - pos;
                size_type new_size = len - len1 + len2;
                if(check_realloc(new_size))
                {
                    __rep* new_ptr = __rep::create(new_size);
                    new_ptr->copy(0, data(), pos);
                    new_ptr->copy(pos + len2, data() + pos + len1, len - pos - len1);
                    new_ptr->copy(pos, str, len2);
                    repup(new_ptr);
                }
                else
                {
                    rep()->move(pos + len2, data() + pos + len1, len - pos - len1);
                    rep()->copy(pos, str, len2);
                }
                rep()->length = new_size;
                return *this;
           }

           base_string& replace(size_type pos, size_type len1, const base_string& other)
           {
                const size_type len = length();
                size_type len2 = other.size();

                if(len1 > len - pos)
                    len1 = len - pos;
                size_type new_size = len - len1 + len2;
                if(check_realloc(new_size))
                {
                    __rep* new_ptr = __rep::create(new_size);
                    new_ptr->copy(0, data(), pos);
                    new_ptr->copy(pos + len2, data() + pos + len1, len - pos - len1);
                    new_ptr->copy(pos, other.data(), len2);
                    repup(new_ptr);
                }
                else
                {
                    rep()->move(pos + len2, data() + pos + len1, len - pos - len1);
                    rep()->copy(pos, other.data(), len2);
                }
                rep()->length = new_size;
                return *this;
           }

           size_type find_first_of(const base_string& str, size_type pos = 0) const
           {
                const size_type len1 = length();
                const size_type len2 = str.size();
                
                if(pos >= len1)
                    return npos;

                for(; pos < len1; ++pos)
                    for(size_type i = 0; i < len2; ++i)
                        if(traits::eq((*rep())[pos], str[i]))
                            return pos;

                return npos;
           }

           size_type find_first_of(const_pointer str, size_type pos = 0) const
           {
                const size_type len1 = length();
                const size_type len2 = traits::length(str);

                if(pos >= len1)
                    return npos;

                for(; pos < len1; ++pos)
                    for(size_type i = 0; i < len2; ++i)
                        if(traits::eq((*rep())[pos], str[i]))
                            return pos;

                return npos;
           }

           size_type find_first_of(const_pointer str, size_type pos, size_type n) const
           {
                const size_type len1 = length();
                const size_type len2 = traits::length(str);

                if(pos >= len1)
                    return npos;

                for(; pos < len1; ++pos)
                    for(size_type i = 0; i < n; ++i)
                        if(traits::eq((*rep())[pos], str[i]))
                            return pos;
                
                return npos;
           }

           size_type find_first_of(value_type c, size_type pos = 0) const
           {
                size_type len1 = length();

                if(pos > len1)
                    return npos;

                for(; pos < len1; ++pos)
                    if(traits::eq((*rep())[pos], c))
                        return pos;

                return npos;
           }

           size_type find(const base_string& str, size_type pos = 0) const
           {
                const size_type len1 = length();
                const size_type len2 = str.length();
                size_type char_left = len1 - pos;

                if(pos > len1 || char_left < len2)
                    return npos;

                for(; pos < len1; ++pos)
                {
                    if(traits::compare(data() + pos, str.data(), len2) == 0)
                        return pos;
                    else if(char_left == len2)
                        return npos;
                    else
                        --char_left;
                }

                return npos;
           }

           size_type find(const_pointer s, size_type pos, size_type n) const
           {
                const size_type len = length();
                size_type char_left = len - pos;

                if(pos > len || char_left < n)
                    return npos;

                for(; pos < len; ++pos)
                {
                    if(traits::compare(data() + pos, s, n) == 0)
                        return pos;
                    else if(char_left == n)
                        return npos;
                    else
                        --char_left;
                }

                return npos;
           }

           size_type find(const_pointer s, size_type pos = 0) const
           {
                const size_type len1 = length();
                const size_type len2 = traits::length(s);
                size_type char_left = len1 - pos;

                if(pos > len1 || char_left < len2)
                    return npos;

                for(; pos < len1; ++pos)
                {
                    if(traits::compare(data() + pos, s, len2) == 0)
                        return pos;
                    else if(char_left == len2)
                        return npos;
                    else
                        --char_left;
                }
                
                return npos;
           }

           size_type find(value_type c, size_type pos = 0) const
           {
                const size_type len1 = length();

                for(; pos < len1; ++pos)
                    if(traits::eq(*(data() + pos), c))
                        return pos;
                return npos;
           }

           size_type rfind(const base_string& str, size_type pos = npos) const
           {
                const size_type len1 = length();
                const size_type len2 = str.length();

                if(len1 < len2)
                    return npos;

                pos = pos > len1? len1 : pos;
                size_type char_right = len1 - pos;

                if(char_right < len2)
                    pos = len1 - len2;
                for(++pos; pos-- > 0;)
                    if(traits::compare(data() + pos, str.data(), len2) == 0)
                        return pos;
                
                return npos;
           }

           size_type rfind(const_pointer str, size_type pos = npos) const
           {
                const size_type len1 = length();
                const size_type len2 = traits::length(str);

                if(len1 < len2)
                    return npos;

                pos = pos > len1? len1 : pos;
                size_type char_right = len1 - pos;

                if(char_right < len2)
                    pos = len1 - len2;
                for(++pos; pos-- > 0;)
                    if(traits::compare(data() + pos, str, len2) == 0)
                        return pos;

                return npos;
           }

           size_type rfind(const_pointer str, size_type pos, size_t n) const
           {
                const size_type len1 = length();

                if(len1 < n)
                    return npos;

                pos = pos > len1? len1 : pos;
                size_type char_right = len1 - pos;

                if(char_right < n)
                    pos = len1 - n;
                for(++pos; pos-- > 0;)
                    if(traits::compare(data() + pos, str, n) == 0)
                        return pos;

                return npos;
           }

           size_type rfind(value_type c, size_type pos) const
           {
                const size_type len1 = length();
                
                if(len1 < 1)
                    return npos;

                pos = pos >= len1? len1 - 1 : pos;
                for(++pos; pos-- > 0;)
                    if(traits::eq(c, *(data() + pos)))
                        return pos;

                return npos;
           }

            // iterator
            iterator begin()
            {
                selfish();

                return &dat[0];
            }

            const_iterator begin() const
            {
                return &dat[0];
            }

            iterator end()
            {
                selfish();

                return &dat[length()];
            }

            const_iterator end() const
            {
                return &dat[length()];
            }

            const_pointer data() const
            {
                return dat;
            }

            // capacity

            size_type  length() const
            {
                return rep()->length;
            }

            size_type size() const
            {
                return rep()->length;
            }

            size_type capacity() const
            {
                return rep()->capacity;
            }

            bool empty() const
            {
                return size() == 0;
            }

            // operator
            reference operator[](size_type n)
            {
                selfish();
                return dat[n];
            }

            const_reference operator[](size_type n) const
            {
                return dat[n];
            }

            const_pointer c_str()
            {
                traits::assign((*rep())[length()], eos());
                return dat;
            }

            // compare
            int compare(const base_string& str, size_type pos, size_type n) const
            {
                size_type rlen = length() - pos;
                if(rlen > n)
                    rlen = n;
                if(rlen > str.length())
                    rlen = str.length();
                int result = traits::compare(data() + pos, str.data(), rlen);
                if(result != 0)
                    return result;
                if(rlen == n)
                    return 0;
                return (length() - pos) - str.length();
            }

            int compare(const_pointer str) const
            {
                const size_type len1 = length();
                const size_type len2 = traits::length(str);
                const size_type min_size = len1 > len2? len2 : len1;

                int result = traits::compare(data(), str, min_size);
                if(result != 0)
                    return result;
                return len1 - len2;
            }

        private:
            __rep *rep() const
            {
                return reinterpret_cast<__rep *>(dat) - 1;
            }

            void repup(__rep *p)
            {
                rep()->release();
                dat = p->data();
            }

            // without selfish
            iterator ibegin() const
            {
                return &dat[0];
            }

            // without selfish
            iterator iend() const
            {
                return &dat[length()];
            }

            bool check_realloc(size_type n) const
            {
                // for c_str
                n += sizeof(value_type);
                rep()->selfish = false;
                return rep()->reference_count > 1 || n > capacity() || __rep::excess_slop(n, capacity());
            }

            void unique()
            {
                if(rep()->reference_count == 1)
                    return;
                alloc(length(), true);
            }

            void selfish()
            {
                unique();
                rep()->selfish = true;
            }

            void alloc(size_t n, bool save)
            {
                if(!check_realloc(n))
                    return;

                __rep* new_ptr = __rep::create(n);
                if(save)
                {
                    new_ptr->copy(0, data(), n);
                    new_ptr->length = n;
                }
                else
                    new_ptr->length = 0;

                repup(new_ptr);
            }

            value_type eos()
            {
                return traits::eos();
            }

            static __rep nil_rep;
        public:
            static const size_type npos;
    };

    template<class charT, class traits, class Alloc>
    typename base_string<charT, traits, Alloc>::__rep base_string<charT, traits, Alloc>::nil_rep = {0, 0, 1, false};

    template<class charT, class traits, class Alloc>
    const size_t base_string<charT, traits, Alloc>::npos = -1;

    template<class charT, class traits, class Alloc>
    bool operator==(const base_string<charT, traits, Alloc>& lhs, const base_string<charT, traits, Alloc>& rhs)
    {
        return lhs.compare(rhs) == 0;
    }

    template<class charT, class traits, class Alloc>
    bool operator==(const base_string<charT, traits, Alloc>& lhs, const char* rhs)
    {
            return lhs.compare(rhs) == 0;
    }

    template<class charT, class traits, class Alloc>
    bool operator==(const char* lhs, const base_string<charT, traits, Alloc>& rhs)
    {
            return rhs.compare(lhs) == 0;
    }

}
#endif
