#ifndef __MINISTL_STL_FUNCTION_H
#define __MINISTL_STL_FUNCTION_H

#include"Functional.h"

namespace MiniStl
{
    template<class T>
    struct identity : public unary_function<T, T>
    {
        const T& operator()(const T& value) const
        {
            return value;
        }
    };
}

#endif
