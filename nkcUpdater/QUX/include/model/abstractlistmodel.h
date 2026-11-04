#ifndef QUX_MODEL_ABSTRACTLISTMODEL_H
#define QUX_MODEL_ABSTRACTLISTMODEL_H

#include "modelitem.h"

#include <QObject>

namespace Qux {
namespace Model {

class AbstractListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    AbstractListModel(QObject* parent)
        : QObject(parent)
    {}
    virtual ~AbstractListModel() {}

    virtual const QList<ModelItem>& list() const = 0;

    virtual ModelItem at(int index) const = 0;
    virtual ModelItem operator[](int index) const { return at(index); }

    virtual int count() const = 0;

signals:
    void inserted(int start, const QList<ModelItem>& list);
    void removed(int start, int end);
    void updated(int start, const QList<ModelItem>& list);
    void countChanged(int count);

};

}
}

#endif