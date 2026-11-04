#ifndef QUX_FUNCTIONTYPE_H
#define QUX_FUNCTIONTYPE_H

#include <functional>

namespace Qux {

template <typename T>
struct UnaryPredicate
{
    typedef std::function<bool (const T& a)> Type;
};

template <typename T>
struct BinaryPredicate
{
    typedef std::function<bool (const T& a, const T& b)> Type;
};

template <typename T>
struct Comparer
{
    typedef std::function<bool (const T& a, const T& b)> Type;
};

template <typename T>
struct UnaryOp
{
    typedef std::function<T (const T& a)> Type;

    template <typename R>
    struct Ret
    {
        typedef std::function<R (const T& a)> Type;
    };
};

}

#endif