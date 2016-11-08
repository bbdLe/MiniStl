#ifndef __TEST_HASHSETTEST_H
#define __TEST_HASHSETTEST_H
#include<cassert>
#include"../Functional.h"
#include"../Hashset.h"
#include"../Hashfunc.h"
#include"../Algobase.h"

using namespace MiniStl;

struct HashsetTest
{
    void test1()
    {
        int array[] = {1, 2, 3, 4, 5, 6, 0, -1};
        hashset<int, hash<int>, equal_to<int>> iset((hash<int>()), equal_to<int>());
        iset.insert(array, array + sizeof(array) / sizeof(int));
        assert(iset.size() == sizeof(array) / sizeof(int));
        assert(iset.insert(1).second == false);
        assert(iset.count(1) == 1);
    }
    
    void test2()
    {
        int array[] = {1, 2, 3, 4, 5, 6, 0, -1};
        hashset<int, hash<int>, equal_to<int>> iset((hash<int>()), equal_to<int>());
        
        iset.erase(0);
        assert(iset.find(0) == iset.end());
        assert(iset.count(0) == 0);
    }

    void test3()
    {
        int array[] = {1, 2, 3, 4, 5, 6, 0, -1, -2, -3, -4, -5};
        hashset<int, hash<int>, equal_to<int>> iset1((hash<int>()), equal_to<int>());
        iset1.insert(array, array + sizeof(array) / sizeof(int));
        hashset<int, hash<int>, equal_to<int>> iset2(iset1);

        assert(distance(iset2.begin(), iset2.end()) == distance(iset1.begin(), iset1.end()));
        assert(iset2.erase(2));
        assert(iset2.count(2) == 0);
        assert(iset2.size() != iset1.size());
        assert(iset2 != iset1);
        using MiniStl::swap;
        swap(iset1, iset2);
        assert(iset1.size() == sizeof(array) / sizeof(int) - 1);
        assert(iset2.size() == sizeof(array) / sizeof(int));
        
    }

    void test()
    {
        test1();
        test2();
        test3();
    }
};


#endif
