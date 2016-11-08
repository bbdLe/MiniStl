#ifndef __MINISTL_HASHFUNC_H
#define __MINISTL_HASHFUNC_H

namespace MiniStl
{
    template<class T>
    struct hash
    {

    };

    size_t hash_for_string(const char* str)
    {
        size_t n = 0;

        for(; *str; ++str)
        {
            n =  5 * n + *str;
        }

        return n;
    }

    template<>
    struct hash<int>
    {
        size_t operator()(int n) const
        {
            return n;
        }
    };

    template<>
    struct hash<unsigned int>
    {
        size_t operator()(unsigned int n) const
        {
            return n;
        }
    };

    template<>
    struct hash<short>
    {
        size_t operator()(short n) const
        {
            return n;
        }
    };

    template<>
    struct hash<unsigned short>
    {
        size_t operator()(unsigned short n) const
        {
            return n;
        }
    };

    template<>
    struct hash<long>
    {
        size_t operator()(long n) const
        {
            return n;
        }
    };

    template<>
    struct hash<unsigned long>
    {
        size_t operator()(unsigned long n) const
        {
            return n;
        }
    };

    template<>
    struct hash<long long>
    {
        size_t operator()(long long n) const
        {
            return n;
        }
    };

    template<>
    struct hash<unsigned long long>
    {
        size_t operator()(unsigned long long n) const
        {
            return n;
        }
    };

    template<>
    struct hash<char*>
    {
        size_t operator()(const char* str) const
        {
            return hash_for_string(str);
        }
    };

    template<>
    struct hash<const char*>
    {
        size_t operator()(const char* str) const
        {
            return hash_for_string(str);
        }
    };
}

#endif
