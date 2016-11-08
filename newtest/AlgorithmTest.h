#ifndef __TEST_TESTALGORITHM_H
#define __TEST_TESTALGORITHM_H
#include"../Algobase.h"
#include"../Algorithm.h"
#include<cassert>

using namespace MiniStl;

struct AlgorithmTest
{
    void test1()
    {
        int array1[] = {1, 2, 5, 4, 3};
        int array2[] = {1, 3, 2, 4, 5};
        int array3[] = {5, 4, 3, 2, 1};
        int array4[] = {1, 2, 3, 4, 5};

        next_permutation(array1, array1 + sizeof(array1) / sizeof(int));
        assert(equal(array1, array1 + sizeof(array1) / sizeof(int), array2));
        next_permutation(array3, array3 + sizeof(array3) / sizeof(int));
        assert(equal(array3, array3 + sizeof(array3) / sizeof(int), array4));
    }

    void test2()
    {
        int array1[] = {1, 2, 3, 5, 4};
        int array2[] = {1, 2, 3, 4, 5};
        int array3[] = {1, 2, 3, 4, 5};
        int array4[] = {5, 4, 3, 2, 1};

        prev_permutation(array1, array1 + sizeof(array1) / sizeof(int));
        assert(equal(array1, array1 + sizeof(array1) / sizeof(int), array2));
        prev_permutation(array3, array3 + sizeof(array3) / sizeof(int));
        assert(equal(array3, array3 + sizeof(array3) / sizeof(int), array4));
    }

    void test3()
    {
        int array[] = {5, 11, 3, 210, 302, 10};

        partial_sort(array, array + 3, array + sizeof(array) / sizeof(int));
        assert(adjacent_find(array, array + 3, [](const int& x, const int& y){return x > y;}) == array + 3);
    }

    void test4()
    {
        int array[] = {5, 11, 3, 210, 302, 10};
        
        vector<int> vec;
        vec.resize(4);
        partial_sort_copy(array, array + sizeof(array) / sizeof(int), vec.begin(), vec.end());
        assert(adjacent_find(vec.begin(), vec.end(), [](const int& x, const int& y){return x > y;}) == vec.end());
    }

    void test5()
    {
        int array[] = {2, 4, 3, 10, 0, -1, 2};

        sort(array, array + sizeof(array) / sizeof(int));
        assert(adjacent_find(array, array + sizeof(array) / sizeof(int), [](const int& x, const int& y){return x > y;}) == array + sizeof(array) / sizeof(int));
    }

    void test6()
    {
        int array[] = {1, 2, 3, 4, 4, 5, 6, 7, 8, 9};

        auto result = equal_range(array, array + sizeof(array) / sizeof(int), 4);
        assert(distance(result.first, result.second) == 2);
        result = equal_range(array, array + sizeof(array) / sizeof(int), 10);
        assert(distance(result.first, result.second) == 0);
    }

    void test7()
    {
        int array1[] = {2, 4, 3, 10, 0, -1, 2};
        
        vector<int> ivec1(array1, array1 + sizeof(array1) / sizeof(int));
        vector<int> ivec2(array1, array1 + sizeof(array1) / sizeof(int));
        sort(ivec1.begin(), ivec1.end());
        nth_element(ivec2.begin(), ivec2.begin() + 3, ivec2.end());
        assert(ivec2[3] == ivec1[3]);
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
