#pragma once

#include "../defines/defineproperty.h"

#include <QObject>


namespace common {
    namespace ux {

        class SignalObject : public QObject
        {
            Q_OBJECT

        public:
            SignalObject(QObject* parent = nullptr)
                : QObject(parent)
            {}

        signals:
            void emitted();

        };

#define VALUEOBJECT(NAME, TYPE, DEFAULT)        \
    class NAME : public QObject {               \
        Q_OBJECT                                \
        PROPERTY(TYPE, value, setValue)         \
    public:                                     \
        NAME(QObject* parent = nullptr)         \
            : m_value(DEFAULT), QObject(parent) \
        {}                                      \
    };

        VALUEOBJECT(BoolObject, bool, false)
            VALUEOBJECT(IntObject, int, 0)
            VALUEOBJECT(QStringObject, QString, "")

#undef VALUEOBJECT

    }

}
