#ifndef __TEST_SETTEST_H
#define __TEST_SETTEST_H
#include"../Set.h"
#include"../Algo.h"
#include"../Algobase.h"
#include"../Utility.h"
#include<cassert>

using namespace MiniStl;

struct SetTest
{
    void test1()
    {
        int array[] = {1, 2, 3, 4, 5, 6, 7, 8};
        set<int> iset(array, array + sizeof(array) / sizeof(int));

        assert(iset.size() == sizeof(array) / sizeof(int));
        assert(iset.insert(1).second == false);
        assert(adjacent_find(iset.begin(), iset.end(), [](int x, int y){return y - x < 0;}) == iset.end());
    }

    void test2()
    {
        int array[] = {3, 2, 1, 4, 5, 6, 7, 0, 8};
        set<int> iset;

        iset.insert(array, array + sizeof(array) / sizeof(int));
        assert(iset.size() == sizeof(array) / sizeof(int));
        assert(adjacent_find(iset.begin(), iset.end(), [](int x, int y){return y - x < 0;}) == iset.end());
        iset.erase(8);
        assert(adjacent_find(iset.begin(), iset.end(), [](int x, int y){return y - x < 0;}) == iset.end());
        iset.clear();
        assert(iset.empty());
    }

    void test3()
    {
        int array[] = {1, 2, 3, 4, 5, 7};
        set<int> iset(array, array + sizeof(array) / sizeof(int));
        set<int> iset1(iset);

        assert(distance(iset.lower_bound(1), iset.upper_bound(1)) == 1);
        assert(iset1 == iset);
        iset.erase(1);
        using MiniStl::swap;
        swap(iset, iset1);
        assert(iset1.size() != sizeof(array) / sizeof(int));
        assert(iset.size() == sizeof(array) / sizeof(int));
    }

    void test()
    {
        test1();
        test2();
        test3();
    }
};


#endif
