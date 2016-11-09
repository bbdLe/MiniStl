#ifndef __TEST_BASESTRINGTEST_H
#define __TEST_BASESTRINGTEST_H
#include"../Basestring.h"
#include"../Algobase.h"
#include"../Algo.h"
#include"../String.h"
#include<iostream>
#include<cassert>
#include<cctype>

using namespace MiniStl;

struct BasestringTest
{
    void test1()
    {
        const char* array1 = "hello";
        const char* array2 = ", world";
        base_string<char> str(array1, array1 + strlen(array1));
        str.replace(str.end(), str.end(), array2, array2 + strlen(array2));
        assert(str.size() == strlen(array1) + strlen(array2));
        assert(equal(str.begin(), str.end(), "hello, world"));
        assert(strlen(str.c_str()) == strlen(array1) + strlen(array2));
        assert(str[4] == 'o');
    }

    void test2()
    {
        const char* array1 = "hello";
        base_string<char> str1(array1, array1 + strlen(array1));
        base_string<char> str2(str1);

        assert(str1.size() == str2.size());
        assert(equal(str1.begin(), str1.end(), str2.begin()));
        str2.replace(str2.end(), str2.end(), array1, array1 + strlen(array1));
        assert(str1.size() != str2.size());
        assert(strlen(str2.c_str()) == 2 * strlen(array1));
    }

    void test3()
    {
        const char* array1 = "hello";
        base_string<char> str1(array1, array1 + strlen(array1));
        
        str1.erase(str1.begin(), str1.begin() + 2);
        assert(equal(str1.begin(), str1.end(), "llo"));
    }

    void test4()
    {
        const char* array = "hello";
        base_string<char> str(array);

        assert(equal(str.begin(), str.end(), array));
            str.erase(2, 3);
        assert(strncmp(str.c_str(), "he", str.size()) == 0);
    }

    void test5()
    {
        const char* array1 = "hello";
        const char* array2 = ", world";
        base_string<char> str1(array1);
        base_string<char> str2(array2);
        
        str1.append(str2);
        assert(equal(str1.begin(), str1.end(), "hello, world"));
    }

    void test6()
    {
        base_string<char> str1("Please, replace the vowels in this sentence by asterisks.");
        base_string<char> str2("aeiou");
        size_t found = str1.find_first_of(str2);
        while(found != base_string<char>::npos)
        {
            str1[found] = '*';
            found = str1.find_first_of(str2, found + 1);
        }
        assert(equal(str1.begin(), str1.end(), "Pl**s*, r*pl*c* th* v*w*ls *n th*s s*nt*nc* by *st*r*sks."));
    }

    void test7()
    {
        base_string<char> str1("Hello, world!Hello, world!Hello, world");
        base_string<char> str2("Hello");
        size_t found = str1.find(str2);
        while(found != base_string<char>::npos)
        {
            str1[found] = '*';
            found = str1.find(str2, found + 1);
        }
        assert(equal(str1.begin(), str1.end(), "*ello, world!*ello, world!*ello, world"));
    }

    void test8()
    {
        base_string<char> str1("Hello, world!Hello, world!Hello, world");
        base_string<char> str2("Hello");
        size_t found = str1.rfind(str2);
        str1[found] = '*';
        assert(equal(str1.begin(), str1.end(), "Hello, world!Hello, world!*ello, world"));
    }

    void test9()
    {
        base_string<char> str("Hello");
        str.clear();
        assert(distance(str.begin(), str.end()) == 0);
        assert(str.empty());
    }

    void test10()
    {
        base_string<char> str("Hello");
        str.insert(1, "ooo");
        assert(equal(str.begin(), str.end(), "Hoooello"));
    }

    void test11()
    {
        base_string<char> str("Hello");
        assert(str == "Hello");
    }

    void test12()
    {
        string str("string");
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
    }
};

#endif
