#ifndef __TEST_HASHMULTISET_H
#define __TEST_HASHMULTISET_H
#include"../Hashmultiset.h"
#include"../Hashfunc.h"
#include"../Functional.h"
#include"../Algobase.h"
#include<iostream>

using namespace MiniStl;

struct HashmultisetTest
{
    void test1()
    {
        int array[] = {1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0};
        hashmultiset<int, hash<int>, equal_to<int>> imset((hash<int>()), equal_to<int>());

        imset.insert(array, array + sizeof(array) / sizeof(int));
        assert(imset.size() == sizeof(array) / sizeof(int));
        assert(imset.count(5) == 2);
        imset.insert(5);
        assert(imset.count(5) == 3);
    }

    void test2()
    {
        const int array[] = {1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0};
        hashmultiset<int, hash<int>, equal_to<int>> imset1((hash<int>()), equal_to<int>());

        imset1.insert(array, array + sizeof(array) / sizeof(int));
        assert(distance(imset1.begin(), imset1.end()) == sizeof(array) / sizeof(int));
        hashmultiset<int, hash<int>, equal_to<int>> imset2(imset1);
        assert(distance(imset1.begin(), imset1.end()) == sizeof(array) / sizeof(int));
        assert(distance(imset2.begin(), imset2.end()) == sizeof(array) / sizeof(int));
        assert(imset2.size() == imset1.size());
        assert(imset1.erase(1) == 2);
        assert(imset1.find(1) == imset1.end());
        assert(imset1.count(1) == 0);
    }

    void test3()
    {
        const int array[] = {1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0};
        hashmultiset<int, hash<int>, equal_to<int>> imset1((hash<int>()), equal_to<int>());

        imset1.insert(array, array + sizeof(array) / sizeof(int));
        hashmultiset<int, hash<int>, equal_to<int>> imset2(imset1);
        assert(imset1 == imset2);
        imset1.erase(1);
        assert(imset1 != imset2);
        using MiniStl::swap;
        swap(imset1, imset2);
        assert(imset2.count(1) == 0);
        assert(imset1.count(1) == 2);
    }

    void test()
    {
        test1();
        test2();
        test3();
    }
};

#endif
