#ifndef __TEST_TESTSLIST_H
#define __TEST_TESTSLIST_H
#include<cassert>
#include<iostream>
#include"../Slist.h"
#include"../Algo.h"

using namespace MiniStl;

struct SlistTest
{
    void test1()
    {
        int array[] = {1, 2, 3, 4, 5};
        slist<int> ilist(array , array + sizeof(array) / sizeof(int));
        assert(ilist.size() == sizeof(array) / sizeof(int));
        ilist.clear();
        assert(ilist.empty());
        ilist.push_front(1);
        ilist.push_front(2);
        assert(ilist.size() == 2);
        ilist.pop_front();
        assert(ilist.size() == 1);
    }

    void test2()
    {
        int array[] = {1, 2, 3, 4, 5};
        slist<int> ilist(array, array + sizeof(array) / sizeof(int));
        ilist.insert(++ilist.begin(), 10);
        assert(*(++ilist.begin()) == 10);
        ilist.erase(++ilist.begin());
        assert(*(++ilist.begin()) == 2);
        ilist.insert(++ilist.begin(), array, array + sizeof(array) / sizeof(int));
        assert(ilist.size() == 10);
        ilist.erase(++ilist.begin(), ++++++++++++ilist.begin());
        assert(ilist.size() == 5);
        assert(adjacent_find(ilist.begin(), ilist.end(), [](int x, int y) {return y - x != 1;}) == ilist.end());
    }

    void test3()
    {
        int array[] = {1, 3, 4, 5};
        slist<int> ilist(array, array + sizeof(array) / sizeof(int));
        ilist.insert_after(ilist.begin(), 2);
        assert(adjacent_find(ilist.begin(), ilist.end(), [](int x, int y) {return y - x != 1;}) == ilist.end());
        ilist.erase_after(ilist.begin());
        assert(*(++ilist.begin()) == 3);
    }

    void test4()
    {
        int array[] = {5, 4, 3, 2, 1};
        slist<int> ilist(array, array + sizeof(array) / sizeof(int));
        ilist.reverse();
        assert(adjacent_find(ilist.begin(), ilist.end(), [](int x, int y) {return y - x != 1;}) == ilist.end());
    }

    void test5()
    {
        int array1[] = {1, 4, 5};
        int array2[] = {2, 3};
        slist<int> ilist1(array1, array1 + sizeof(array1) / sizeof(int));
        slist<int> ilist2(array2, array2 + sizeof(array2) / sizeof(int));
        ilist1.splice(++ilist1.begin(), ilist2);
        assert(adjacent_find(ilist1.begin(), ilist1.end(), [](int x, int y) {return y - x != 1;}) == ilist1.end());
        assert(ilist2.empty());
        ilist2.insert(ilist2.begin(), array2, array2 + sizeof(array2) / sizeof(int));
        ilist1.splice(++ilist1.begin(), ilist2, ilist2.begin());
        assert(ilist2.size() == 1);
        assert(*(++ilist1.begin()) == 2);
        assert(*(++++ilist1.begin()) == 2);
    }

    void test6()
    {
        int array1[] = {1, 2, 3, 4};
        int array2[] = {0, 5, 6, 7, 8};
        slist<int> ilist1(array1, array1 + sizeof(array1) / sizeof(int));
        slist<int> ilist2(array2, array2 + sizeof(array2) / sizeof(int));
        ilist1.merge(ilist2);
        assert(ilist2.empty());
        assert(ilist1.front() == 0);
        assert(ilist1.back() == 8);
        assert(adjacent_find(ilist1.begin(), ilist1.end(), [](int x, int y) {return y - x <= 0;}) == ilist1.end());
    }

    void test7()
    {
        int array1[] = {1, 1, 3, 2, 5, 4, 3, 3, 4, 4, 5};
        slist<int> ilist(array1, array1 + sizeof(array1) / sizeof(int));
        ilist.sort();
        ilist.unique();
        assert(ilist.size() == 5);
    }

    void test8()
    {
        int array[] = {2, 32, 12, 5, 346, 451, 123, 687};
        slist<int> ilist(array, array + sizeof(array) / sizeof(int));
        ilist.sort();
        assert(ilist.front() == 2);
        assert(ilist.back() == 687);
        assert(adjacent_find(ilist.begin(), ilist.end(), [](int x, int y) {return y - x <= 0;}) == ilist.end());
    }

    void test9()
    {
        int array[] = {1, 2, 3, 4};
        slist<int> ilist1(array, array + sizeof(array) / sizeof(int));
        slist<int> ilist2(array, array + sizeof(array) / sizeof(int));
        assert(ilist2 == ilist1);
        ilist1.erase(ilist1.begin());
        assert(ilist1 != ilist2);
        using MiniStl::swap;
        swap(ilist1, ilist2);
        assert(ilist2.front() == 2);
    }

    void test10()
    {
        int array[] = {1, 2, 3, 4, 10, 2, 3};

        slist<int> ilist(array, array + sizeof(array) / sizeof(int));
        assert(ilist.size() == sizeof(array) / sizeof(int));
        ilist.resize(20);
        assert(ilist.size() == 20);
        ilist.remove([](int x) {return x <= 4;});
        assert(ilist.size() == 1);
    }


    void test()
    {
        test1();
        test2();
        test3();
        test4();
        test5();
        test6();
        test7();
        test8();
        test9();
        test10();
    }
};


#endif
