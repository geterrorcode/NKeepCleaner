#pragma once
#include "list.h"

#include <QVariant>

namespace Common {

template <typename T>
QList<QVariant> toQVariantList(const QList<T>& list)
{
    return convertList<QVariant>(list, &QVariant::fromValue<T>);
}

template <typename T>
QList<T> fromQVariantList(const QVariantList& list)
{
    return convertList<T>(list, [] (const QVariant& value) { return value.value<T>(); });
}

QVariantMap toQvm(const QString& key0, const QVariant& value0);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6, const QString& key7, const QVariant& value7);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6, const QString& key7, const QVariant& value7,
                  const QString& key8, const QVariant& value8);
QVariantMap toQvm(const QString& key0, const QVariant& value0, const QString& key1, const QVariant& value1,
                  const QString& key2, const QVariant& value2, const QString& key3, const QVariant& value3,
                  const QString& key4, const QVariant& value4, const QString& key5, const QVariant& value5,
                  const QString& key6, const QVariant& value6, const QString& key7, const QVariant& value7,
                  const QString& key8, const QVariant& value8, const QString& key9, const QVariant& value9);

}
