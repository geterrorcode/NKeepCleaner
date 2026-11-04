#ifndef QUX_MODEL_KEYMODEL_H
#define QUX_MODEL_KEYMODEL_H

#include "abstractkeymodel.h"

namespace Qux {
namespace Model {

class KeyModel : public AbstractKeyModel
{
    Q_OBJECT

public:
    KeyModel(QObject* parent = nullptr);

    void clear();
    void insert(const QList<QPair<ModelItem, ModelItem>>& pairs);
    void insert(const ModelItem& key, const ModelItem& value);
    void remove(const ModelItem& key);

    const QMap<ModelItem, ModelItem>& map() const override { return m_map; };

    bool contains(const ModelItem& key) const { return m_map.contains(key); }
    ModelItem value(const ModelItem& key) const { return m_map.value(key); }

    int count() const { return m_map.size(); }

private:
    QMap<ModelItem, ModelItem> m_map;

};

}
}

#endif