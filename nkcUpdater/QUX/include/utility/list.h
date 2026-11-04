#ifndef QUX_UTILITY_LIST_H
#define QUX_UTILITY_LIST_H

#include <QList>

#include <algorithm>
#include <iterator>

namespace Qux {

template <typename T, typename List, typename UnaryOp>
QList<T> convertList(const List& list, UnaryOp op)
{
    QList<T> result;
    std::transform(list.cbegin(), list.cend(), std::back_inserter(result), op);
    return result;
}

}

#endif
