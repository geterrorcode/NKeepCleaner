#ifndef QUX_UTILITY_MAP_H
#define QUX_UTILITY_MAP_H

#include <QMap>

namespace Qux {

template <typename Key, typename T>
QMap<Key, T> mergeMap(const QMap<Key, T>& target, const QMap<Key, T>& source)
{
    QMap<Key, T> map(target);
    for (auto iter = source.cbegin(); iter != source.cend(); iter++) {
        map[iter.key()] = iter.value();
    }
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0)
{
    QMap<Key, T> map;
    map[Key0] = value0;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3,
                   const Key& Key4, const T& value4)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3; map[Key4] = value4;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3,
                   const Key& Key4, const T& value4, const Key& Key5, const T& value5)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3; map[Key4] = value4; map[Key5] = value5;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3,
                   const Key& Key4, const T& value4, const Key& Key5, const T& value5,
                   const Key& Key6, const T& value6)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3; map[Key4] = value4; map[Key5] = value5;
    map[Key6] = value6;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3,
                   const Key& Key4, const T& value4, const Key& Key5, const T& value5,
                   const Key& Key6, const T& value6, const Key& Key7, const T& value7)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3; map[Key4] = value4; map[Key5] = value5;
    map[Key6] = value6; map[Key7] = value7;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3,
                   const Key& Key4, const T& value4, const Key& Key5, const T& value5,
                   const Key& Key6, const T& value6, const Key& Key7, const T& value7,
                   const Key& Key8, const T& value8)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3; map[Key4] = value4; map[Key5] = value5;
    map[Key6] = value6; map[Key7] = value7; map[Key8] = value8;
    return map;
}

template <typename Key, typename T>
QMap<Key, T> toMap(const Key& Key0, const T& value0, const Key& Key1, const T& value1,
                   const Key& Key2, const T& value2, const Key& Key3, const T& value3,
                   const Key& Key4, const T& value4, const Key& Key5, const T& value5,
                   const Key& Key6, const T& value6, const Key& Key7, const T& value7,
                   const Key& Key8, const T& value8, const Key& Key9, const T& value9)
{
    QMap<Key, T> map;
    map[Key0] = value0; map[Key1] = value1; map[Key2] = value2;
    map[Key3] = value3; map[Key4] = value4; map[Key5] = value5;
    map[Key6] = value6; map[Key7] = value7; map[Key8] = value8;
    map[Key9] = value9;
    return map;
}

}

#endif
