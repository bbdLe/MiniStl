#ifndef __TEST_TESTQUEUE_H
#define __TEST_TESTQUEUE_H
#include"../Queue.h"
#include<cassert>
#include"../Utility.h"

using MiniStl::queue; 

struct QueueTest
{
    void test1()
    {
        queue<int> ique;

        ique.push(5);
        ique.push(4);
        ique.push(3);
        ique.push(2);
        ique.push(1);
        
        assert(ique.size() == 5);
        assert(ique.front() == 5);
        assert(ique.back() == 1);
        ique.pop();
        assert(ique.front() == 4);
        ique.pop();
        assert(ique.front() == 3);
        ique.pop();
        ique.pop();
        ique.pop();
        assert(ique.empty());
    }

    void test2()
    {
        int array1[] = {1, 2, 3};
        int array2[] = {1};

        queue<int> ique1(array1, array1 + sizeof(array1) / sizeof(int));
        queue<int> ique2(array2, array2 + sizeof(array2) / sizeof(int));
        using MiniStl::swap;
        swap(ique1, ique2);
        assert(ique1.size() == sizeof(array2) / sizeof(int));
        assert(ique2.size() == sizeof(array1) / sizeof(int));
    }

    void test()
    {
        test1();
        test2();
    }
};

#endif
