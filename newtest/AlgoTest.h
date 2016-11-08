#ifndef __TEST_ALGOTEST_H
#define __TEST_ALGOTEST_H
#include"../Algo.h"
#include"../Vector.h"
#include<cassert>

struct AlgoTest
{
    void test1()
    {
        int array1[] = {1, 2, 3, 4, 5, 6};
        int array2[] = {1, 2, 3, 4, 4};

        vector<int> ivec1(array1, array1 + sizeof(array1) / sizeof(int));
        vector<int> ivec2(array2, array2 + sizeof(array2) / sizeof(int));
        assert(adjacent_find(ivec1.begin(), ivec1.end()) == ivec1.end());
        assert(adjacent_find(ivec2.begin(), ivec2.end()) != ivec2.end());
    }

    void test2()
    {
        int array1[] = {1, 2, 3, 4, 4, 5, 6};
        
        assert(count(array1, array1 + sizeof(array1) / sizeof(int), 4) == 2);
        assert(count(array1, array1 + sizeof(array1) / sizeof(int), 1) == 1);
        assert(count(array1, array1 + sizeof(array1) / sizeof(int), 0) == 0);
    }

    void test3()
    {
        int array[] = {1, 5, 4, 3, 2, 10};
        vector<int> vec(array, array + sizeof(array) / sizeof(int));
        
        assert(find(vec.begin(), vec.end(), 5) != vec.end());
        assert(find(vec.begin(), vec.end(), 11) == vec.end());
    }

    void test4()
    {
        int array1[] = {1, 2, 3, 4, 5, 6, 7};
        int array2[] = {4, 5, 6};
        int array3[] = {4, 5, 5};

        vector<int> vec(array1, array1 + sizeof(array1) / sizeof(int));
        assert(search(vec.begin(), vec.end(), array2, array2 + sizeof(array2) / sizeof(int)) != vec.end());
        assert(equal(array2, array2 + sizeof(array2) / sizeof(int), search(vec.begin(), vec.end(), array2, array2 + sizeof(array2) / sizeof(int))));
        assert(search(vec.begin(), vec.end(), array3, array3 + sizeof(array3) / sizeof(int)) == vec.end());
    }

    void test5()
    {
        int array1[] = {1, 2, 3, 3, 2, 1, 1, 2, 3};
        int array2[] = {1, 2, 3};

        vector<int> vec(array1, array1 + sizeof(array1) / sizeof(int));
        assert(find_end(vec.begin(), vec.end(), array2, array2 + sizeof(array2) / sizeof(int)) == vec.begin() + 6);
    }

    void test6()
    {
        std::string str1("aeiou");
        std::string str2("hello");

        assert(find_first_of(str2.begin(), str2.end(), str1.begin(), str1.end()) != str2.end());
        assert(*find_first_of(str2.begin(), str2.end(), str1.begin(), str1.end()) == 'e');
    }

    void test7()
    {
        struct func
        {
            int count = 1;

            void operator()(const int& i)
            {
                assert(i == count);
                ++count;
            }
        };

        int array[] = {1, 2, 3, 4};
        for_each(array, array + sizeof(array) / sizeof(int), func());
    }

    void test8()
    {
        struct func
        {
            int count = 1;

            int operator()()
            {
                return count++;
            }
        };

        vector<int> vec(10);
        generate(vec.begin(), vec.end(), func());
        assert(adjacent_find(vec.begin(), vec.end(), [](const int& x, const int& y) {return y - x != 1;}) == vec.end());
    }

    void test9()
    {
        int array1[] = {1, 2, 3, 4, 5, 6, 7, 9};
        int array2[] = {1, 4, 9};
        int array3[] = {0, 4, 6};

        assert(includes(array1, array1 + sizeof(array1) / sizeof(int), array2, array2 + sizeof(array2) / sizeof(int)) == true);
        assert(includes(array1, array1 + sizeof(array1) / sizeof(int), array3, array3 + sizeof(array3) / sizeof(int)) == false);
    }

    void test10()
    {
        int array[] = {1, 0, 4, 3, 2, 5, 6, 0, 9};

        assert(*max_element(array, array + sizeof(array) / sizeof(int)) == 9);
    }

    void test11()
    {
        int array[] = {1, 0, 4, 3, 2, 5, 6, 0, 9};

        assert(*min_element(array, array + sizeof(array) / sizeof(int)) == 0);
    }

    void test12()
    {
        int array1[] = {1, 3, 5, 7, 9};
        int array2[] = {2, 4, 6, 8};
        vector<int> ivec;

        merge(array1, array1 + sizeof(array1) / sizeof(int), array2, array2 + sizeof(array2) / sizeof(int), back_inserter(ivec));
        assert(adjacent_find(ivec.begin(), ivec.end(), [](const int& x, const int& y){return x > y;}) == ivec.end());
    }
    
    void test13()
    {
        int array[] = {1, 11, 2, 22, 3, 33, 4, 44, 5, 55, 6, 66};

        int* position = partition(array, array + sizeof(array) / sizeof(int), [](const int& x){return x >=10;});
        assert(find_if(position, array + sizeof(array) / sizeof(int), [](const int& x){return x >= 10;}) == array + sizeof(array) / sizeof(int));
        assert(find_if(array, position, [](const int& x){return x < 10;}) == position);
    }

    void test14()
    {
        int array[] = {1, 11, 2, 22, 3, 33, 4, 44, 5, 55};
        vector<int> ivec(array, array + sizeof(array) / sizeof(int));

        ivec.erase(remove_if(ivec.begin(), ivec.end(), [](const int& x){return x > 10;}), ivec.end());
        assert(find_if(ivec.begin(), ivec.end(), [](const int& x){return x > 10;}) == ivec.end());
        assert(ivec.size() == 5);
    }

    void test15()
    {
        int array[] = {1, 11, 2, 22, 3, 33, 4, 44, 5, 55};
        vector<int> ivec(array, array + sizeof(array) / sizeof(int));

        replace_if(ivec.begin(), ivec.end(), [](const int& x){return x > 10;}, 5);
        assert(find_if(ivec.begin(), ivec.end(), [](const int& x){return x > 10;}) == ivec.end());
    }

    void test16()
    {
        int array[] = {1, 2, 3, 4, 5, 6};
        vector<int> ivec(array, array + sizeof(array) / sizeof(int));
        
        reverse(ivec.begin(), ivec.end());
        assert(adjacent_find(ivec.begin(), ivec.end(), [](const int& x, const int& y){return x - y != 1;}) == ivec.end());
    }

    void test17()
    {
        int array[] = {3, 2, 1, 4, 5, 6, 7};
        
        vector<int> ivec(array, array + sizeof(array) / sizeof(int));
        rotate(ivec.begin(), ivec.begin() + 3, ivec.end());
        assert(adjacent_find(ivec.begin(), ivec.begin() + 4, [](const int& x, const int& y){return y - x != 1;}) == ivec.begin() + 4);
        assert(adjacent_find(ivec.begin() + 4, ivec.end(), [](const int& x, const int& y){return x - y != 1;}) == ivec.end());
    }

    void test18()
    {
        int array[] = {1, 2, 2, 2, 3, 3, 3, 4};

        vector<int> ivec(array, array + sizeof(array) / sizeof(int));
        assert(search_n(ivec.begin(), ivec.end(), 3, 2) != ivec.end());
        assert(search_n(ivec.begin(), ivec.end(), 4, 3) == ivec.end());
    }

    void test19()
    {
        int array[] = {1, 2, 2, 3, 4, 4, 5, 5, 6, 7, 8};

        vector<int> ivec;
        unique_copy(array, array + sizeof(array) / sizeof(int), back_inserter(ivec));
        assert(adjacent_find(ivec.begin(), ivec.end(), [](const int& x, const int& y){return x == y;}) == ivec.end());
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
        test11();
        test12();
        test13();
        test14();
        test15();
        test16();
        test17();
        test18();
        test19();
    }
};


#endif
