#ifndef __TEST_ALGOBASETEST_H
#define __TEST_ALGOBASETEST_H
#include<cassert>
#include<string>
#include"../Algobase.h"
#include"../Iterator.h"
#include"../Algo.h"
#include"../Vector.h"

using namespace MiniStl;

struct AlgobaseTest
{
    void test1()
    {
        int array[] = {1, 2, 3, 3, 4, 5, 6, 7, 8};
        vector<int> ivec(array, array + sizeof(array) / sizeof(int));

        assert(distance(lower_bound(ivec.begin(), ivec.end(), 3), upper_bound(ivec.begin(), ivec.end(), 3)) == 2);
        assert(distance(lower_bound(ivec.begin(), ivec.end(), 4), upper_bound(ivec.begin(), ivec.end(), 4)) == 1);
        assert(distance(lower_bound(ivec.begin(), ivec.end(), 10), upper_bound(ivec.begin(), ivec.end(), 10)) == 0);
    }

    void test2()
    {
        int array1[] = {1, 2, 3, 4, 5};
        int array2[] = {1, 2, 3, 5, 4};
        
        assert(mismatch(array1, array1 + sizeof(array1) / sizeof(int), array2).first == array1 + 3);
        assert(mismatch(array1, array1 + sizeof(array1) / sizeof(int), array2).second == array2 + 3);
    }
    
    void test3()
    {
        int array1[] = {1, 2, 3, 4, 5};
        int array2[] = {1, 2, 3, 5, 4};

        assert(equal(array1, array1 + sizeof(array1) / sizeof(int), array1) == true);
        assert(equal(array1, array1 + sizeof(array1) / sizeof(int), array2) == false);
    }

    void test4()
    {
        const std::string str1("Hello");
        const std::string str2("Hello!");
        const std::string str3("Hallo");

        assert(MiniStl::lexicographical_compare(str1.begin(), str1.end(), str2.begin(), str2.end()) == true);
        assert(MiniStl::lexicographical_compare(str1.begin(), str1.end(), str1.begin(), str1.begin()) == false);
        assert(MiniStl::lexicographical_compare(str1.begin(), str1.end(), str3.begin(), str3.begin()) == false);
    }

    void test5()
    {
        int array1[] = {1, 2, 3, 4, 5};
        int array2[] = {1, 3, 4, 5};
        int array3[] = {1, 2, 3, 4, 5};
        vector<int> result;

        set_union(array1, array1 + sizeof(array1) / sizeof(int), array2, array2 + sizeof(array2) / sizeof(int), back_inserter(result));
        assert(equal(result.begin(), result.end(), array3));
    }

    void test6()
    {
        int array1[] = {1, 2, 3, 4, 5, 6};
        int array2[] = {0, 1, 3, 6, 8};
        int array3[] = {1, 3, 6};
        vector<int> result;

        set_intersection(array1, array1 + sizeof(array1) / sizeof(int), array2, array2 + sizeof(array2) / sizeof(int), back_inserter(result));
        assert(equal(result.begin(), result.end(), array3));
    }

    void test7()
    {
        int array1[] = {2, 3, 4, 5, 6, 7};
        int array2[] = {2, 4, 6};
        int array3[] = {3, 5, 7};
        vector<int> result;

        set_difference(array1, array1 + sizeof(array1) / sizeof(int), array2, array2 + sizeof(array2) / sizeof(int), back_inserter(result));
        assert(equal(result.begin(), result.end(), array3));
    }

    void test8()
    {
        int array1[] = {1, 3, 5, 7, 9};
        int array2[] = {2, 4, 5, 7, 10};
        int array3[] = {1, 2, 3, 4, 9, 10};
        vector<int> result;

        set_symmetric_difference(array1, array1 + sizeof(array1) / sizeof(int), array2, array2 + sizeof(array2) / sizeof(int), back_inserter(result));
        assert(equal(result.begin(), result.end(), array3));
    }

    void test9()
    {
        int array[] = {1, 3, 5, 7, 9, 10};
        vector<int> vec(array, array + sizeof(array) / sizeof(int));
        assert(binary_search(vec.begin(), vec.end(), 3) == true);
        assert(binary_search(vec.begin(), vec.end(), 8) == false);
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
    }
};


#endif
