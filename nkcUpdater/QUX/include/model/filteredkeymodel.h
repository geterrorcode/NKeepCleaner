#ifndef QUX_MODEL_FILTEREDKEYMODEL_H
#define QUX_MODEL_FILTEREDKEYMODEL_H

#include "abstractkeymodel.h"
#include "../defineproperty.h"
#include "../functiontype.h"

namespace Qux {
namespace Model {

class FilteredKeyModel : public AbstractKeyModel
{
    Q_OBJECT

    PROPERTY_NOCOMP(UnaryPredicate<ModelItem>::Type, filter, setFilter)

public:
    FilteredKeyModel(const AbstractKeyModel* model, QObject* parent = nullptr);

    const QMap<ModelItem, ModelItem>& map() const override { return m_filteredMap; };

    bool contains(const ModelItem& key) const { return m_filteredMap.contains(key); }
    ModelItem value(const ModelItem& key) const { return m_filteredMap.value(key); }

    int count() const { return m_filteredMap.size(); }

private:
    QMap<ModelItem, ModelItem> m_filteredMap;

};

}
}

#endif