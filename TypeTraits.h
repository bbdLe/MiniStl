#ifndef __MINISTL_TYPE_TRITS_H_
#define __MINISTL_TYPE_TRITS_H_

namespace MiniStl
{
    struct _true_type {};
    struct _false_type {};

    // 萃取器
    template <class T>
    struct _type_traits
    {
        typedef _false_type has_trival_default_constructor;
        typedef _false_type has_trival_copy_constructor;
        typedef _false_type has_trival_assignment_operator;
        typedef _false_type has_trival_destructor;
        typedef _false_type is_POD_type;
    };

    // 对内置类型特例化
    template <>
    struct _type_traits<char>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<unsigned char>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<signed char>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };


    template <>
    struct _type_traits<short>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<unsigned short>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<int>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<unsigned int>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<long>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<unsigned long>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<long long>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<unsigned long long>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<float>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<double>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <>
    struct _type_traits<long double>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    // 对指针进行偏例化
    template <class T>
    struct _type_traits<T *>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };

    template <class T>
    struct _type_traits<const T *>
    {
        typedef _true_type has_trival_default_constructor;
        typedef _true_type has_trival_copy_constructor;
        typedef _true_type has_trival_assignment_operator;
        typedef _true_type has_trival_destructor;
        typedef _true_type is_POD_type;
    };
}
#endif
