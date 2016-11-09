#ifndef __MINISTL_STRAITS_H
#define __MINISTL_STRAITS_H

#include<cctype>
#include<cstring>
#include"Vector.h"

namespace MiniStl
{
    template<class charT>
    struct string_traits
    {
        typedef charT char_type;

        static void assign(char_type& c1, const char_type& c2)
        {
            c1 = c2;
        }

        static bool eq(const char_type& c1, const char_type& c2)
        {
            return c1 == c2;
        }

        static bool ne(const char_type& c1, const char_type& c2)
        {
            return c1 != c2;
        }

        static bool lt(const char_type& c1, const char_type& c2)
        {
            return c1 < c2;
        }

        static char_type eos()
        {
            return char_type();
        }

        static bool is_del(const char_type& c)
        {
            return false;
        }

        static int compare(const char_type* str1, const char_type* str2, size_t n)
        {
            for(; n > 0; --n, ++str1, ++str2)
            {
                if(ne(*str1, *str2))
                {
                    return lt(*str1, *str2)? -1 : 1;
                }
            }

            return 0;
        }

        static size_t length(const char_type* str)
        {
            size_t count = 0;

            for(; ne(*str, eos); ++count, ++str)
                ;
            return count;
        }

        static char_type* copy(char_type* str1, const char_type* str2, size_t n)
        {
            for(size_t i = 0; i < n; ++i)
                assign(str1[i], str2[i]);

            return str1;
        }

        static char_type* move(char_type* str1, const char_type* str2, size_t n)
        {
            vector<char_type> temp(str2, str2 + n);

            for(size_t i = 0; i < n; ++i)
                assign(str1[i], temp[i]);

            return str1;
        }

        static char_type* set(char_type* str, char_type c, size_t n)
        {
            for(size_t i = 0; i < n; ++i)
                assign(str[i], c);

            return str;
        }
    };

    template<>
    struct string_traits<char>
    {
        typedef char char_type;

        static void assign(char_type& c1, const char_type& c2)
        {
            c1 = c2;
        }

        static bool eq(const char_type& c1, const char_type& c2)
        {
            return c1 == c2;
        }

        static bool ne(const char_type& c1, const char_type& c2)
        {
            return c1 != c2;
        }

        static bool lt(const char_type& c1, const char_type& c2)
        {
            return c1 < c2;
        }

        static char_type eos()
        {
            return 0;
        }

        static bool is_del(const char_type& c)
        {
            return isspace(c);
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n)
        {
            return memcmp(s1, s2, n);
        }

        static size_t length(const char_type* str)
        {
            return strlen(str);
        }

        static char_type* copy(char_type* s1, const char_type* s2, size_t n)
        {
            return static_cast<char_type*>(memcpy(s1, s2, n));
        }

        static char_type* move(char_type* s1, const char_type* s2, size_t n)
        {
            return static_cast<char_type*>(memmove(s1, s2, n));
        }

        static char_type* set(char_type* str, char_type c, size_t n)
        {
            return static_cast<char_type*>(memset(str, c, n));
        }
    };
}

#endif
