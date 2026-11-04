#ifndef QUX_UTILITY_QVALUE_H
#define QUX_UTILITY_QVALUE_H

#include <QObject>

namespace Common
{

    template <typename T>
    void toQValue(T* pt, QObject* parent)
    {
        auto life = new QObject(parent);
        QObject::connect(life, &QObject::destroyed, [pt]() {
            delete pt;
            });
    }

    template <typename T>
    T* createQValue(QObject* parent)
    {
        auto pt = new T;
        toQValue(pt, parent);
        return pt;
    }

    template <typename T>
    T* createQValue(const T& value, QObject* parent)
    {
        auto pt = new T(value);
        toQValue(pt, parent);
        return pt;
    }

    

}

#endif