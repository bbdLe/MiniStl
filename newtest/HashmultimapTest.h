#ifndef __TEST_HASHMULTIMAP_H
#define __TEST_HASHMULTIMAP_H
#include"../Hashmultimap.h"
#include"../Hashfunc.h"
#include"../Functional.h"
#include"../Utility.h"
#include"../Algobase.h"
#include"../Algo.h"
#include<cassert>

using namespace MiniStl;

struct HashmultimapTest
{
    typedef pair<int, bool> value_type;
    void test1()
    {
        value_type array[] = {{1, true}, {2, true}, {1, false}, {3, false}, {0, true}};
        hashmultimap<int, bool, hash<int>, equal_to<int>> imap((hash<int>()), equal_to<int>());
        imap.insert(array, array + sizeof(array) / sizeof(value_type));
        assert(imap.size() == sizeof(array) / sizeof(value_type));
        assert(distance(imap.begin(), imap.end()) == sizeof(array) / sizeof(value_type));
    }
    
    void test2()
    {
        value_type array[] = {{1, true}, {2, true}, {1, false}, {3, false}, {0, true}};
        hashmultimap<int, bool, hash<int>, equal_to<int>> imap((hash<int>()), equal_to<int>());
        
        imap.insert(array, array + sizeof(array) / sizeof(value_type));
        assert(imap.count(1) == 2);
        imap.insert(make_pair(1, false));
        assert(imap.count(1) == 3);
        imap.erase(1);
        assert(imap.count(1) == 0);
    }

    void test3()
    {
        value_type array[] = {{1, true}, {2, true}, {1, false}, {3, false}, {0, true}};
        hashmultimap<int, bool, hash<int>, equal_to<int>> imap((hash<int>()), equal_to<int>());
        imap.insert(array, array + sizeof(array) / sizeof(value_type));
        hashmultimap<int, bool, hash<int>, equal_to<int>> imap2(imap);
        assert(distance(imap2.begin(), imap2.end()) == sizeof(array) / sizeof(value_type));
        assert(distance(imap.begin(), imap.end()) == sizeof(array) / sizeof(value_type));
        assert(imap2 == imap);
        assert(distance(imap.begin(), imap.end()) == distance(imap2.begin(), imap2.end()));
        imap2.erase(1);
        assert(imap2 != imap);
        using MiniStl::swap;
        swap(imap2, imap);
        assert(imap2.size() == sizeof(array) / sizeof(value_type));
    }

    void test()
    {
        test1();
        test2();
        test3();
    }
};

#endif

