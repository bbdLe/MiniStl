#ifndef __TEST_STACKTEST_H
#define __TEST_STACKTEST_H
#include"../Stack.h"
#include<cassert>

struct StackTest
{
    void test1()
    {
        int array[] = {1, 2, 3, 4, 5};

        stack<int> istak(array, array + sizeof(array) / sizeof(int));

        assert(istak.top() == 5);
        istak.pop();
        assert(istak.top() == 4);
        istak.pop();
        istak.push(10);
        assert(istak.top() == 10);
        istak.pop();
        istak.pop();
        istak.pop();
        assert(istak.top() == 1);
        assert(istak.size() == 1);
    }

    void test()
    {
        test1();
    }
};
#endif
