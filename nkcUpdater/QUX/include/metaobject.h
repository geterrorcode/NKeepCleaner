#ifndef QUX_METAOBJECT_H
#define QUX_METAOBJECT_H

#include <QMetaMethod>
#include <QString>

namespace Qux {

QMetaMethod findMethodByName(const QObject* object, const QString& name, QMetaMethod::MethodType type);
QMetaMethod findSignalByName(const QObject* object, const QString& name);
QMetaMethod findSlotByName(const QObject* object, const QString& name);

}

#endif