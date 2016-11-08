#ifndef __MINISTL_UTILITY_H
#define __MINISTL_UTILITY_H
namespace MiniStl
{
    template<class T1, class T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

        pair(const T1 &value1, const T2 &value2) : first(value1), second(value2) {}

        pair(const pair& rhs) : first(rhs.first), second(rhs.second) {}

        template<class U1, class U2>
        pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

        pair& operator=(const pair& rhs)
        {
            first = rhs.first;
            second = rhs.second;
            return *this;
        }

        bool operator==(const pair& rhs) const
        {
            return first == rhs.first && second == rhs.second;
        }

        bool operator!=(const pair& rhs) const
        {
            return !(operator==(rhs));
        }
    };

    template<class T1, class T2>
    pair<T1, T2> make_pair(T1 x, T2 y)
    {
        return pair<T1, T2>(x, y);
    }

    template<class T>
    void swap(T& lhs, T& rhs)
    {
        T temp = lhs;
        lhs = rhs;
        rhs = temp;
    }
}


#endif
