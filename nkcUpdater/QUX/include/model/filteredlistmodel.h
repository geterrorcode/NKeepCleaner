#ifndef QUX_MODEL_FILTEREDLISTMODEL_H
#define QUX_MODEL_FILTEREDLISTMODEL_H

#include "abstractlistmodel.h"
#include "../defineproperty.h"
#include "../functiontype.h"

namespace Qux {
namespace Model {

class FilteredListModel : public AbstractListModel
{
    Q_OBJECT

    PROPERTY_NOCOMP(UnaryPredicate<ModelItem>::Type, filter, setFilter)

public:
    FilteredListModel(const AbstractListModel* model, QObject* parent = nullptr);

    const QList<ModelItem>& list() const override { return m_filteredList; };

    ModelItem at(int index) const override { return m_filteredList.at(index); }

    int count() const override { return m_filteredList.count(); }

private:
    QList<ModelItem> m_filteredList;

};

}
}

#endif