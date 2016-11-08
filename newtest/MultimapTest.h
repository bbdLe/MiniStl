#ifndef __TEST_MULTIMAPTEST_H
#define __TEST_MULTIMAPTEST_H
#include"../Multimap.h"
#include"../Utility.h"
#include"../Algo.h"
#include"../Algobase.h"
#include<iostream>

using namespace MiniStl;

struct MultimapTest
{
    typedef pair<int, bool> value_type;
    void test1()
    {
        value_type array[] = {{1, false}, {1, true}, {2, true}, {4, true}, {3, false}, {5, false}, {5, true}};
        multimap<int, bool> imap(array, array + sizeof(array) / sizeof(value_type));

        assert(imap.size() == sizeof(array) / sizeof(value_type));
        assert(adjacent_find(imap.begin(), imap.end(), [](const value_type& x, const value_type& y)
                    {return select1st<value_type>()(x) > select1st<value_type>()(y);}) == imap.end());
        imap.insert({3, false});
        assert(distance(imap.lower_bound(3), imap.upper_bound(3)) == 2);
    }

    void test2()
    {
        value_type array[] = {{1, false}, {1, true}, {2, true}, {4, true}, {3, false}, {5, false}, {5, true}};

        multimap<int, bool> imap(array, array + sizeof(array) / sizeof(value_type));
        imap.erase(1);
        assert(imap.size() == sizeof(array) / sizeof(value_type) - 2);
        assert(imap.find(1) == imap.end());
        assert(distance(imap.lower_bound(1), imap.upper_bound(1)) == 0);
    }

    void test3()
    {
        value_type array[] = {{1, false}, {1, true}, {2, true}, {4, true}, {3, false}, {5, false}, {5, true}};
        multimap<int, bool> imap(array, array + sizeof(array) / sizeof(value_type));
        
        multimap<int, bool> imap1(imap);
        assert(imap1 == imap);
        imap.erase(1);
        assert(imap != imap1);
        using MiniStl::swap;
        swap(imap, imap1);
        assert(imap.size() == sizeof(array) / sizeof(value_type));
    }

    void test()
    {
        test1();
        test2();
        test3();
    }
};

#endif
