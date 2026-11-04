#ifndef QUX_MODEL_ABSTRACTKEYMODEL_H
#define QUX_MODEL_ABSTRACTKEYMODEL_H

#include "modelitem.h"

#include <QObject>

namespace Qux {
namespace Model {

class AbstractKeyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    AbstractKeyModel(QObject* parent)
        : QObject(parent)
    {}
    virtual ~AbstractKeyModel() {}

    virtual const QMap<ModelItem, ModelItem>& map() const = 0;

    virtual bool contains(const ModelItem& key) const = 0;
    virtual ModelItem value(const ModelItem& key) const = 0;
    virtual ModelItem operator[](const ModelItem& key) const { return value(key); }

    virtual int count() const = 0;

signals:
    void inserted(const QList<QPair<ModelItem, ModelItem>>& pairs);
    void removed(const QList<ModelItem> keys);
    void updated(const QList<QPair<ModelItem, ModelItem>>& pairs);
    void countChanged(int count);

};

}
}

#endif