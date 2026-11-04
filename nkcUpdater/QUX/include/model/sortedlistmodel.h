#ifndef QUX_MODEL_SORTEDLISTMODEL_H
#define QUX_MODEL_SORTEDLISTMODEL_H

#include "abstractlistmodel.h"
#include "../defineproperty.h"
#include "../functiontype.h"

namespace Qux {
namespace Model {

class SortedListModel : public AbstractListModel
{
    Q_OBJECT

    PROPERTY_NOCOMP(Comparer<ModelItem>::Type, comparer, setComparer)

public:
    SortedListModel(const AbstractListModel* model, QObject* parent = nullptr);

    const QList<ModelItem>& list() const override { return m_sortedList; };

    ModelItem at(int index) const override { return m_sortedList.at(index); }

    int count() const override { return m_sortedList.size(); }

private:
    QList<ModelItem> m_sortedList;

};

}
}

#endif