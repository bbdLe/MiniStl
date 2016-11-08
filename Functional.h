#ifndef __MINISTL_FUNCTIONAL_H
#define __MINISTL_FUNCTIONAL_H

namespace MiniStl
{
    template<class Arg, class Result>
    struct unary_function
    {
        typedef Arg arugment_type;
        typedef Result result_type;
    };

    template<class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    template<class T>
    struct plus : binary_function<T, T, T>
    {
        T operator()(const T& value1, const T& value2) const
        {
            return value1 + value2;
        }
    };

    template<class T>
    struct minus : binary_function<T, T, T>
    {
        T operator()(const T& value1, const T& value2) const
        {
            return value1 - value2;
        }
    };

    template<class T>
    struct multiplies : binary_function<T, T, T>
    {
        T operator()(const T& value1, const T& value2) const
        {
            return value1 * value2;
        }
    };

    template<class T>
    struct divides : binary_function<T, T, T>
    {
        T operator()(const T& value1, const T& value2) const
        {
            return value1 / value2;
        }
    };

    template<class T>
    struct modulus : binary_function<T, T, T>
    {
        T operator()(const T& value1, const T& value2) const
        {
            return value1 % value2;
        }
    };

    template<class T>
    struct negate : unary_function<T, T>
    {
        T operator()(const T& value) const
        {
            return -value;
        }
    };

    template<class T>
    struct equal_to: binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 == value2;
        }
    };

    template<class T>
    struct not_equal_to : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 != value2;
        }
    };

    template<class T>
    struct greater : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 > value2;
        }
    };

    template<class T>
    struct less : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 < value2;
        }
    };

    template<class T>
    struct greater_equal : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 >= value2;
        }
    };

    template<class T>
    struct less_equal : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 <= value2;
        }
    };

    template<class T>
    struct logical_and : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 && value2;
        }
    };

    template<class T>
    struct logical_or : binary_function<T, T, bool>
    {
        bool operator()(const T& value1, const T& value2) const
        {
            return value1 || value2;
        }
    };

    template<class T>
    struct logical_not : unary_function<T, bool>
    {
        bool operator()(const T& value)
        {
            return !value;
        }
    };

    template<class Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type>
    {
        const typename Pair::first_type& operator()(const Pair& p) const
        {
            return p.first;
        }
    };

    template<class Pair>
    struct select2st : public unary_function<Pair, typename Pair::second_type>
    {
        const typename Pair::second_type& operator()(const Pair& p) const
        {
            return p.second;
        }
    };

    template<class T>
    inline T identity_element(plus<T>)
    {
        return T(0);
    }

    template<class T>
    inline T identity_element(multiplies<T>)
    {
        return T(1);
    }

    template<class T1, class T2>
    struct project1st : binary_function<T1, T2, T1>
    {
        T1 operator()(const T1& value1, const T2&)
        {
            return value1;
        }
    };

    template<class T1, class T2>
    struct project2st : binary_function<T1, T2, T2>
    {
        T2 operator()(const T1&, const T2& value2)
        {
            return value2;
        }
    };
    
    //
    template<class Predicate>
    class unary_negate : unary_function<typename Predicate::arguement_type, bool>
    {
        private:
            Predicate pred;

        public:
            unary_negate(const Predicate& p) : pred(p) {}

            bool operator()(const typename Predicate::argument_type& value) const
            {
                return !pred(value);
            }
    };

    template<class Predicate>
    inline unary_negate<Predicate> not1(const Predicate& p)
    {
        return unary_negate<Predicate>(p);
    }

    template<class Predicate>
    class binary_negate : binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool>
    {
        private:
            Predicate pred;

        public:
            binary_negate(const Predicate& p) : pred(p) {}

            bool operator()(const typename Predicate::first_argument_type& first, const typename Predicate::second_argument_type& second) const
            {
                return !pred(first, second);
            }
    };

    template<class Operator, class T>
    class binder1st : unary_function<typename Operator::second_argument_type, typename Operator::result_type>
    {
        private:
            Operator func;
            T val1;
        
        public:
            binder1st(const Operator& op, const T& v) : func(op), val1(v) {}
            
            typename Operator::result_type operator()(const typename Operator::second_argument_type& val2)
            {
                return func(val1, val2);
            }
    };

    template<class Operator, class T>
    inline binder1st<Operator, T> bind1st(const Operator& op, const T& value)
    {
        return binder1st<Operator, T>(op, static_cast<typename Operator::first_argument_type>(value));
    }

    template<class Operator, class T>
    class binder2st : unary_function<typename Operator::first_argument_type, typename Operator::result_type>
    {
        private:
            Operator func;
            T val1;
        
        public:
            binder2st(const Operator& op, const T& v) : func(op), val1(v) {}

            typename Operator::result_type operator()(const typename Operator::first_argument_type& val2) const
            {
                return func(val1, val2);
            }
    };

    template<class Operator, class T>
    inline binder2st<Operator, T> bind2st(const Operator& op, const T& value)
    {
        return binder2st<Operator, T>(op, static_cast<typename Operator::second_argument_type>(value));
    }

    template<class Operator1, class Operator2>
    class composer1 : unary_function<typename Operator2::argument_type, typename Operator2::result_type>
    {
        private:
            Operator1 op1;
            Operator2 op2;

        public:
            composer1(const Operator1& x, const Operator2& y) : op1(x), op2(y) {}

            typename Operator1::result_type operator()(const typename Operator2::argument_type& value) const
            {
                return op1(op2(value));
            }
    };

    template<class Operator1, class Operator2>
    inline composer1<Operator1, Operator2> compose1(const Operator1& x, const Operator2& y)
    {
        return composer1<Operator1, Operator2>(x, y);
    }

    template<class Operator1, class Operator2, class Operator3>
    class composer2 : unary_function<typename Operator2::argument_type, typename Operator1::result_type>
    {
        private:
            Operator1 op1;
            Operator2 op2;
            Operator3 op3;

        public:
            composer2(const Operator1& x, const Operator2& y, const Operator3& z) : op1(x), op2(y), op3(z) {}
            
            typename Operator1::result operator()(const typename Operator2::argument_type& value) const
            {
                return op1(op2(value), op3(value));
            }

    };

    template<class Operator1, class Operator2, class Operator3>
    inline composer2<Operator1, Operator2, Operator3> compose2(const Operator1& x, const Operator2& y, const Operator3& z)
    {
        return composer2<Operator1, Operator2, Operator3>(x, y, z);
    }

    template<class Arg, class Result>
    class pointer_to_unary_function : unary_function<Arg, Result>
    {
        private:
            Result (*pfunc)(Arg);

        public:
            pointer_to_unary_function(Result (*p)(Arg)) : pfunc(p) {}
            
            Result operator()(Arg argu) const
            {
                return pfunc(argu);
            }
    };

    template<class Arg, class Result>
    inline pointer_to_unary_function<Arg, Result> ptr_func(Result (*p)(Arg))
    {
        return pointer_to_unary_function<Arg, Result>(p);
    }

    template<class Arg1, class Arg2, class Result>
    class pointer_to_binary_function : binary_function<Arg1, Arg2, Result>
    {
        private:
            Result (*pfunc)(Arg1, Arg2);

        public:
            pointer_to_binary_function(Result (*p)(Arg1, Arg2)) : pfunc(p) {}
            Result operator()(Arg1 x, Arg2 y) const
            {
                return pfunc(x, y);
            }
    };

    template<class Arg1, class Arg2, class Result>
    inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_func(Result (*p)(Arg1, Arg2))
    {
        return pointer_to_binary_function<Arg1, Arg2, Result>(p);
    }
    
    template<class T, class Result>
    class mem_fun_t : unary_function<T*, Result>
    {
        private:
            Result (T::*pfunc)();

        public:
            explicit mem_fun_t(Result (T::*p)()) : pfunc(p) {}

            Result operator()(T* p) const
            {
                return (p->*pfunc)();
            }
    };

    template<class T, class Result>
    class const_mem_fun_t : unary_function<const T*, Result>
    {
        private:
            Result (T::*pfunc)() const;

        public:
            explicit const_mem_fun_t(Result (T::*p)() const) : pfunc(p) {}

            Result operator()(const T* p) const
            {
                return (p->*pfunc)();
            }
    };

    template<class T, class Result>
    class mem_fun_ref_t : unary_function<T, Result>
    {
        private:
            Result (T::*pfunc)();

        public:
            explicit mem_fun_ref_t(Result (T::*p)()) : pfunc(p) {}

            Result operator()(T& ref) const
            {
                return (ref.*pfunc)(); 
            }
    };

    template<class T, class Result>
    class const_mem_fun_ref_t : unary_function<T, Result>
    {
        private:
            Result (T::*pfunc)() const;

        public:
            explicit const_mem_fun_ref_t(Result (T::*p)() const) : pfunc(p) {}

            Result operator()(T& ref) const
            {
                return (ref.*pfunc)();
            }
    };

    template<class T, class Result, class Arg>
    class mem_fun1_t : binary_function<T*, Arg, Result>
    {
        private:
            Result (T::*pfunc)(Arg);

        public:
            explicit mem_fun1_t(Result (T::*p)(Arg)) : pfunc(p) {}

            Result operator()(T* p, Arg arg) const
            {
                return (p->*pfunc)(arg);
            }
    };

    template<class T, class Result, class Arg>
    class const_mem_fun1_t : binary_function<const T*, Arg, Result>
    {
        private:
            Result (T::*pfunc)(Arg) const;

        public:
            explicit const_mem_fun1_t(Result (T::*p)(Arg) const) : pfunc(p) {}

            Result operator()(const T* p, Arg arg) const
            {
                return (p->*pfunc)(arg);
            }
    };

    template<class T, class Result, class Arg>
    class mem_fun1_ref_t : binary_function<T, Arg, Result>
    {
        private:
            Result (T::*pfun)(Arg);

        public:
            mem_fun1_ref_t(Result (T::*p)(Arg)) : pfun(p) {}

            Result operator()(T& ref, Arg arg)
            {
                return (ref.*pfun)(arg);
            }
    };

    template<class T, class Result, class Arg>
    class const_mem_fun1_ref_t : binary_function<T, Arg, Result>
    {
        private:
            Result (T::*pfunc)(Arg) const;

        public:
            explicit const_mem_fun1_ref_t(Result (T::*p)(Arg) const) : pfunc(p) {}

            Result operator()(T& ref, Arg arg) const
            {
                return (ref.*pfunc)(arg);
            }
    };

    template<class T, class Result>
    inline mem_fun_t<T, Result> mem_fun(Result (T::*ptr)())
    {
        return mem_fun_t<T, Result>(ptr);
    }

    template<class T, class Result>
    inline const_mem_fun_t<T, Result> mem_fun(Result (T::*ptr)() const)
    {
        return const_mem_fun_t<T, Result>(ptr);
    }

    template<class T, class Result>
    inline mem_fun_ref_t<T, Result> mem_fun_ref(Result (T::*ptr)())
    {
        return mem_fun_ref_t<T, Result>(ptr);
    }

    template<class T, class Result>
    inline const_mem_fun_ref_t<T, Result> mem_fun_ref(Result (T::*ptr)() const)
    {
        return const_mem_fun_t<T, Result>(ptr);
    }

    template<class T, class Result, class Arg>
    inline mem_fun1_t<T, Result, Arg> mem_fun(Result (T::*ptr)(Arg))
    {
        return mem_fun1_t<T, Result, Arg>(ptr);
    }

    template<class T, class Result, class Arg>
    inline const_mem_fun1_t<T, Result, Arg> mem_fun(Result (T::*ptr)(Arg) const)
    {
        return const_mem_fun1_t<T, Result, Arg>(ptr);
    }

    template<class T, class Result, class Arg>
    inline mem_fun1_ref_t<T, Result, Arg> mem_fun_ref(Result (T::*ptr)(Arg))
    {
        return mem_fun1_ref_t<T, Result, Arg>(ptr);
    }

    template<class T, class Result, class Arg>
    inline const_mem_fun1_ref_t<T, Result, Arg> mem_fun_ref(Result (T::*ptr)(Arg) const)
    {
        return const_mem_fun1_t<T, Result, Arg>(ptr);
    }

}
#endif
