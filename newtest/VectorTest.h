#ifndef __TEST_VECTORTEST_H
#define __TEST_VECTORTEST_H
#include"../Vector.h"
#include"../Algo.h"
#include<cassert>

using namespace MiniStl;

class VectorTest
{
    public:
        void test1()
        {
            int arr[] = {1, 2, 3};

            vector<int> ivec(arr, arr + 3);
            assert(ivec.size() == 3);
            assert(ivec.front() = 1);
            assert(ivec[2] = 2);
            assert(ivec.back() = 3);
        }

        void test2()
        {
            int arr[] = {1, 2, 3, 4};

            vector<int> ivec;
            ivec.assign(arr, arr + 4);
            assert(ivec.size() == 4);
            assert(ivec.front() == 1);
            assert(ivec.back() == 4);
        }

        void test3()
        {
            vector<int> ivec(10, 20);

            assert(ivec.size() == 10);
            assert(find_if(ivec.begin(), ivec.end(), [](int value) {return value != 20;}) == ivec.end());
        }

        void test4()
        {
            vector<int> ivec;
            
            ivec.push_back(1);
            ivec.push_back(3);
            assert(ivec.size() == 2);
            assert(ivec.back() == 3);
            assert(ivec.front() == 1);
            ivec.reserve(50);
            assert(ivec.capacity() == 50);
            ivec.resize(50);
            assert(ivec.size() == 50);
            ivec.resize(10);
            assert(ivec.size() == 10);
            ivec.shrink_to_fit();
            assert(ivec.capacity() == 10);
        }

        void test5()
        {
            int array[] = {1, 2, 3, 4, 5};

            vector<int> ivec(array, array + 5);
            ivec.insert(ivec.begin() + 3, 7);
            // 1 2 3 7 4 5
            assert(ivec.size() == 6);
            assert(ivec[3] == 7);
            ivec.insert(ivec.begin() + 5, array, array + 5);
            // 1 2 3 7 4 1 2 3 4 5 5
            assert(ivec.size() == 11);
            assert(ivec[5] == 1);
            assert(ivec[9] == 5);
        }

        void test6()
        {
            int array[] = {1, 2, 3, 4, 5};

            vector<int> ivec(array, array + 5);
            ivec.push_back(6);
            ivec.erase(ivec.begin(), ivec.begin() + 2);
            assert(ivec.size() == 4);
            assert(ivec.front() == 3);
            ivec.erase(ivec.begin() + 1);
            assert(ivec[1] == 5);
        }

        void test7()
        {
            int array[] = {1, 2, 3};

            vector<int> ivec1(array, array + 3);
            vector<int> ivec2;
            using MiniStl::swap;
            swap(ivec1, ivec2);
            assert(ivec1.empty());
            assert(ivec2.size() == 3);
        }

        void test8()
        {
            int array[] = {1, 2, 3};

            vector<int> ivec1(array, array + 3);
            vector<int> ivec2(array, array + 3);
            assert(ivec2 == ivec1);
            assert((ivec1 != ivec2) == false);
        }

        void test9()
        {
            int array[] = {1, 2, 3};

            vector<int> ivec(array, array + 3);
            ivec.insert(ivec.begin(), size_t(3), 3);
            assert(ivec.size() == 6);
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
