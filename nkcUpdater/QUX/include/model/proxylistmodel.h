#ifndef QUX_MODEL_PROXYLISTMODEL_H
#define QUX_MODEL_PROXYLISTMODEL_H

#include "abstractlistmodel.h"
#include "../defineproperty.h"
#include "../functiontype.h"

namespace Qux {
namespace Model {

class ProxyListModel : public AbstractListModel
{
    Q_OBJECT

    PROPERTY_NOCOMP(UnaryPredicate<ModelItem>::Type, filter, setFilter)
    PROPERTY_NOCOMP(Comparer<ModelItem>::Type, comparer, setComparer)

public:
    ProxyListModel(const AbstractListModel* model, QObject* parent = nullptr);

    const QList<ModelItem>& list() const override { return m_outputModel->list(); };

    ModelItem at(int i) const override { return m_outputModel->at(i); }

    int count() const override { return m_outputModel->count(); }

private:
    AbstractListModel* m_outputModel;

};

}
}

#endif