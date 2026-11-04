#ifndef QUX_MODEL_CONVERTEDKEYMODEL_H
#define QUX_MODEL_CONVERTEDKEYMODEL_H

#include "abstractkeymodel.h"
#include "../defineproperty.h"
#include "../functiontype.h"

namespace Qux {
namespace Model {

class ConvertedKeyModel : public AbstractKeyModel
{
    Q_OBJECT

    PROPERTY_NOCOMP(UnaryOp<ModelItem>::Type, converter, setConverter)

public:
    ConvertedKeyModel(const AbstractKeyModel* model, QObject* parent = nullptr);

    const QMap<ModelItem, ModelItem>& map() const override { return m_convertedMap; };

    bool contains(const ModelItem& key) const { return m_convertedMap.contains(key); }
    ModelItem value(const ModelItem& key) const { return m_convertedMap.value(key); }

    int count() const { return m_convertedMap.size(); }

private:
    QMap<ModelItem, ModelItem> m_convertedMap;

};

}
}

#endif