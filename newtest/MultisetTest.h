#ifndef __TEST_MULTISETTEST_H
#define __TEST_MULTISETTEST_H
#include"../Multiset.h"
#include"../Algobase.h"
#include<cassert>

using namespace MiniStl;

struct MultisetTest
{
    void test1()
    {
        int array[] = {3, 3, 2, 1, 1, 4, 5, 6, 8, 8, 7, 9, 0};
        multiset<int> iset(array, array + sizeof(array) / sizeof(int));
        assert(iset.size() == sizeof(array) / sizeof(int));
        assert(iset.count(3) == 2);
        assert(iset.count(1) == 2);
        iset.erase(1);
        assert(iset.count(1) == 0);
        iset.erase(iset.begin(), iset.end());
        assert(iset.size() == 0);
        assert(iset.empty());
    }

    void test2()
    {
        int array[] = {3, 3, 2, 1, 1, 4, 5, 6, 8, 8, 7, 9, 0};
        multiset<int> iset(array, array + sizeof(array) / sizeof(int));

        iset.clear();
        assert(iset.empty());
    }

    void test()
    {
        test1();
        test2();
    }
};


#endif
