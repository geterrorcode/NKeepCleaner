#pragma once

#include <QList>

namespace Common {

template <typename List, typename UnaryOp>
List mapList(const List& list, UnaryOp op)
{
    return convertList<List::value_type>(list, op);
}

template <typename T, typename List>
QList<T> convertList(const List& list)
{
    return convertList<T>(list, [] (const List::value_type& value) { return value; });
}

template <typename T, typename List, typename UnaryOp>
QList<T> convertList(const List& list, UnaryOp op)
{
    QList<T> result;
    std::transform(list.cbegin(), list.cend(), std::back_inserter(result), op);
    return result;
}

template <typename List, typename UnaryOp>
List filterList(const List& list, UnaryOp op)
{
    List result;
    for (auto iter = list.cbegin(); iter != list.cend(); iter++) {
        if (op(*iter)) result.append(*iter);
    }
    return result;
}

template <typename List>
List reverseList(const List& list)
{
    List result(list);
    std::reverse(result.begin(), result.end());
    return result;
}

template <typename T, typename UnaryOp>
T find(const QList<T>& list, UnaryOp op)
{
    auto result = std::find_if(list.cbegin(), list.cend(), op);
    return result != list.cend() ? *result : T();
}

template <typename List, typename UnaryOp>
int findIndex(const List& list, UnaryOp op)
{
    auto result = std::find_if(list.cbegin(), list.cend(), op);
    return result != list.cend() ? result - list.cbegin() : -1;
}

template <typename T, typename It>
QList<T> generateList(It first, It last)
{
    return QList<T>::fromStdList(std::list<T>(first, last));
}

template <typename T>
QList<T> toList(const T& value0)
{
    QList<T> list;
    list << value0;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1)
{
    QList<T> list;
    list << value0 << value1;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2)
{
    QList<T> list;
    list << value0 << value1 << value2;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2,
                const T& value3)
{
    QList<T> list;
    list << value0 << value1 << value2 <<
         value3;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2,
                const T& value3, const T& value4)
{
    QList<T> list;
    list << value0 << value1 << value2 <<
         value3 << value4;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2,
                const T& value3, const T& value4, const T& value5)
{
    QList<T> list;
    list << value0 << value1 << value2 <<
         value3 << value4 << value5;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2,
                const T& value3, const T& value4, const T& value5,
                const T& value6)
{
    QList<T> list;
    list << value0 << value1 << value2 <<
         value3 << value4 << value5 <<
         value6;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2,
                const T& value3, const T& value4, const T& value5,
                const T& value6, const T& value7)
{
    QList<T> list;
    list << value0 << value1 << value2 <<
         value3 << value4 << value5 <<
         value6 << value7;
    return list;
}

template <typename T>
QList<T> toList(const T& value0, const T& value1, const T& value2,
                const T& value3, const T& value4, const T& value5,
                const T& value6, const T& value7, const T& value8)
{
    QList<T> list;
    list << value0 << value1 << value2 <<
         value3 << value4 << value5 <<
         value6 << value7 << value8;
    return list;
}

}

