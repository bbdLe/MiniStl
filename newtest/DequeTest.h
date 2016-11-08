#ifndef __TEST_DEQUETEST_H
#define __TEST_DEQUETEST_H
#include"../Deque.h"
#include"../Algo.h"
#include<iostream>

struct DequeTest
{
    void test1()
    {
        int array[] = {1, 4, 5};
        deque<int> ideq(array, array + sizeof(array) / sizeof(int));
        assert(ideq.size() == sizeof(array) / sizeof(int));
        int arr[] = {2, 3};
        ideq.insert(ideq.begin() + 1, arr, arr + sizeof(arr) / sizeof(int));
        assert(adjacent_find(ideq.begin(), ideq.end(), [](int x, int y){return y - x != 1;}) == ideq.end());
    }

    void test2()
    {
        int array[] = {1, 2, 3, 4, 5};
        deque<int> ideq(array, array + sizeof(array) / sizeof(int));
        ideq.pop_back();
        ideq.pop_front();
        assert(ideq.size() == 3);
        assert(ideq.front() == 2);
        assert(ideq.back() == 4);
    }

    void test3()
    {
        int array[] = {1, 2, 3, 4, 5};
        deque<int> ideq(array, array + sizeof(array) / sizeof(int));
        ideq.erase(ideq.begin(), ideq.end());
        assert(ideq.empty());
    }

    void test4()
    {
        int array[] = {1, 2, 3, 4, 5};
        deque<int> ideq(array, array + sizeof(array) / sizeof(int));
        ideq.resize(500);
        assert(ideq.size() == 500);
        ideq.clear();
        assert(ideq.size() == 0);
        assert(ideq.empty());
    }

    void test5()
    {
        deque<int> ideq;
        deque<int>::const_iterator iter1 = ideq.begin();
        deque<int>::const_iterator iter2 = iter1;
    }

    void test()
    {
        test1();
        test2();
        test3();
        test4();
        test5();
    }
};

#endif
