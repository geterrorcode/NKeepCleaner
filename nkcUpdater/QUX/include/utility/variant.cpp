#include "../utility/variant.h"
#include "../utility/map.h"

namespace Qux {

QVariantMap toQvm(const QString& key0, const QVariant& value0)
{
    return toMap(key0, value0);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1)
{
    return toMap(key0, value0, key1, value1);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2)
{
    return toMap(key0, value0, key1, value1, key2, value2);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3, key4, value4);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3, key4, value4, key5, value5);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3, key4, value4, key5, value5,
                 key6, value6);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6, const QString& key7, const QVariant& value7)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3, key4, value4, key5, value5,
                 key6, value6, key7, value7);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6, const QString& key7, const QVariant& value7,
                  const QString& key8, const QVariant& value8)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3, key4, value4, key5, value5,
                 key6, value6, key7, value7, key8, value8);
}

QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6, const QString& key7, const QVariant& value7,
                  const QString& key8, const QVariant& value8, const QString& key9, const QVariant& value9)
{
    return toMap(key0, value0, key1, value1, key2, value2,
                 key3, value3, key4, value4, key5, value5,
                 key6, value6, key7, value7, key8, value8,
                 key9, value9);
}

}