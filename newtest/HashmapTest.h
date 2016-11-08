#ifndef __TEST_HASHMAPTEST_H
#define __TEST_HASHMAPTEST_H
#include"../Hashmap.h"
#include"../Algobase.h"
#include"../Utility.h"
#include"../Hashfunc.h"
#include"../Functional.h"
#include<cassert>

struct HashmapTest
{
    typedef pair<int, bool> value_type;

    void test1()
    {
        pair<int, bool> array[] = {{1, true}, {2, false}, {3, false}, {0, false}, {-1, false}};
        hashmap<int, bool, hash<int>, equal_to<int>> hmap((hash<int>()), equal_to<int>());
        
        hmap.insert(array, array + sizeof(array) / sizeof(value_type));
        assert(hmap.size() == sizeof(array) / sizeof(value_type));
        assert(distance(hmap.begin(), hmap.end()) == sizeof(array) / sizeof(value_type));
        assert(hmap.insert({1, true}).second == false);
        assert(hmap.count(1) == 1);
    }

    void test2()
    {
        pair<int, bool> array[] = {{1, true}, {2, false}, {3, false}, {0, false}, {-1, false}};
        hashmap<int, bool, hash<int>, equal_to<int>> hmap1((hash<int>()), equal_to<int>());
        hmap1.insert(array, array + sizeof(array) / sizeof(value_type));
     
        hashmap<int, bool, hash<int>, equal_to<int>> hmap2(hmap1);
        assert(hmap1 == hmap2);
        hmap1.erase(hmap1.begin());
        assert(hmap1 != hmap2);
        hmap1.clear();
        assert(hmap1.empty());
        assert(distance(hmap1.begin(), hmap1.end()) == 0);
    }

    void test()
    {
        test1();
        test2();
    }
};

#endif
