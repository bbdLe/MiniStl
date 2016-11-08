#ifndef __TEST_LISTTEST_H
#define __TEST_LISTTEST_H
#include"../List.h"
#include"../Algo.h"
#include<cassert>

using namespace MiniStl;

class ListTest
{
    public:
        void test1()
        {
            list<int> ilist;
            assert(ilist.empty());
            ilist.push_front(1);
            ilist.push_back(2);
            ilist.push_back(3);
            ilist.push_back(4);
            ilist.push_back(5);
            assert(ilist.front() == 1);
            assert(ilist.back() == 5);
            // 1 2 3 4 5
            ilist.erase(++++ilist.begin(), ilist.end());
            // 1 2
            assert(ilist.size() == 2);
            assert(distance(ilist.rbegin(), ilist.rend()) == 2);
            assert(ilist.front() == 1);
            assert(ilist.back() == 2);
        }

        void test2()
        {
            int array[] = {1, 2, 3};
            list<int> ilist(array, array + 3);
            assert(ilist.size() == 3);
            ilist.insert(++ilist.begin(), array, array + 3);
            assert(ilist.size() == 6);
            assert(distance(ilist.rbegin(), ilist.rend()) == 6);
            ilist.insert(ilist.begin(), size_t(3), 10);
            assert(ilist.size() == 9);
            assert(distance(ilist.rbegin(), ilist.rend()) == 9);
            ilist.clear();
            assert(ilist.empty());
        }

        void test3()
        {
            int array1[] = {1, 2, 5};
            int array2[] = {3, 4, 6};
            list<int> ilist1(array1, array1 + 3);
            list<int> ilist2(array2, array2 + 3);
            ilist1.merge(ilist2);
            assert(ilist1.size() == 6);
            assert(ilist2.empty());
            assert(ilist1.front() == 1);
            assert(ilist1.back() == 6);
        }

        void test4()
        {
            int array1[] = {1, 2, 3};
            int array2[] = {4, 5};
            list<int> ilist1(array1, array1 + 3);
            list<int> ilist2(array2, array2 + 2);
            ilist1.splice(ilist1.begin(), ilist2.begin(), ++ilist2.begin());
            assert(ilist1.front() == 4);
            assert(ilist1.size() == 4);
            ilist1.splice(ilist1.begin(), ilist2);
            assert(ilist1.front() == 5);
            assert(ilist1.size() == 5);
        }

        void test5()
        {
            int array[] = {3, 4, 1, 2, 3, 7, 8, 4, 6, 5, 6, 9, 1, 0, 1, 3};
            list<int> ilist(array, array + sizeof(array) / sizeof(int));
            ilist.sort();
            ilist.unique();
            assert(ilist.size() == 10);
            assert(ilist.front() == 0);
            assert(ilist.back() == 9);
            assert(adjacent_find(ilist.begin(), ilist.end(), 
                                [](int x, int y) {return y - x != 1;}) == ilist.end());
            ilist.reverse();
            assert(adjacent_find(ilist.begin(), ilist.end(),
                                [](int x, int y) {return x - y != 1;}) == ilist.end());
        }

        void test6()
        {
            int array[] = {1, 2, 3, 4, 1, 2, 3, 3, 4, 4, 4, 5, 6};
            list<int> ilist(array, array + sizeof(array) / sizeof(int));
            ilist.remove(3);
            assert(find(ilist.begin(), ilist.end(), 3) == ilist.end());
        }

        void test7()
        {
            int array1[] = {1, 2, 3, 4, 5, 6};
            int array2[] = {7, 8, 9, 10, 11, 12};
            
            list<int> ilist1(array1, array1 + sizeof(array1) / sizeof(int));
            list<int> ilist2(array2, array2 + sizeof(array2) / sizeof(int));
            assert(ilist1 != ilist2);
            using MiniStl::swap;
            swap(ilist1, ilist2);
            assert(ilist1.front() == 7);
            assert(ilist2.front() == 1);
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
        }
};
 
#endif
