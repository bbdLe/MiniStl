#ifndef __TEST_MAPTEST_H
#define __TEST_MAPTEST_H
#include<cassert>
#include"../Map.h"
#include"../Utility.h"
#include"../Algo.h"
#include"../Algobase.h"

using namespace MiniStl;

struct MapTest
{
    typedef pair<int, bool> value_type;
    void test1()
    {
        pair<int, bool> array[] = {{1, true}, {2, false}, {5, false}, {3, false}, {4, true}, {7, true}, {6, false}, {8, true}};
        map<int, bool> imap(array, array + sizeof(array) / sizeof(pair<int, bool>));
        assert(imap.size() == sizeof(array) / sizeof(pair<int, bool>));
        assert(imap.insert(make_pair(1, true)).second == false);
        imap.insert(make_pair(10, false));
        assert(adjacent_find(imap.begin(), imap.end(), [](const value_type& x, const value_type& y)
                            {return select1st<value_type>()(x) > select1st<value_type>()(y);}) == imap.end());
    }

    void test2()
    {
        pair<int, bool> array[] = {{1, true}, {2, false}, {5, false}, {3, false}, {4, true}, {7, true}, {6, false}, {8, true}};
        map<int, bool> imap(array, array + sizeof(array) / sizeof(pair<int, bool>));
        imap.erase(1);
        assert(imap.find(1) == imap.end());
        imap.insert({2, false});
        assert(distance(imap.lower_bound(2), imap.upper_bound(2)) == 1);
    }

    void test3()
    {
        pair<int, bool> array[] = {{1, true}, {2, false}, {5, false}, {3, false}, {4, true}, {7, true}, {6, false}, {8, true}};
        map<int, bool> imap(array, array + sizeof(array) / sizeof(pair<int, bool>));
        map<int, bool> imap1(imap);

        assert(imap == imap1);
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
