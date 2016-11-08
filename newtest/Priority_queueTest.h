#ifndef __TEST_PRIORITY_QUEUE_TEST_H
#define __TEST_PRIORITY_QUEUE_TEST_H
#include"../Priority_queue.h"
#include"../Algobase.h"
#include"../Functional.h"
#include<cassert>

using MiniStl::priority_queue;

struct Priority_queueTest
{
    void test1()
    {
        int array[] = {3, 5, 6, 1, 2, 4, 9, 7, 8, 10};
        priority_queue<int> ipqueue(array, array + sizeof(array) / sizeof(int));
        assert(ipqueue.size() == 10);
        assert(ipqueue.top() == 10);
        ipqueue.pop();
        assert(ipqueue.top() == 9);
        for(int i = ipqueue.size(); i > 0; --i)
            ipqueue.pop();
        assert(ipqueue.empty());
    }

    void test2()
    {
        int array[] = {3, 5, 6, 1, 2, 4, 9, 7, 8, 10};
        priority_queue<int> ipqueue(array, array + sizeof(array) / sizeof(int));
        ipqueue.push(20);
        assert(ipqueue.size() == 11);
        assert(ipqueue.top() == 20);
    }

    void test3()
    {
        int array[] = {3, 5, 6, 1, 2, 4, 9, 7, 8, 10};
        priority_queue<int, MiniStl::greater<int>> ipqueue(array, array + sizeof(array) / sizeof(int));
        assert(ipqueue.size() == 10);
        assert(ipqueue.top() == 1);
        ipqueue.push(-10);
        assert(ipqueue.top() == -10);
    }

    void test()
    {
        test1();
        test2();
        test3();
    }
};

#endif
