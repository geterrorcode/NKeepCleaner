#ifndef QUX_MODEL_KEYMODELREFERENCELISTMODEL_H
#define QUX_MODEL_KEYMODELREFERENCELISTMODEL_H

#include "abstractlistmodel.h"
#include "abstractkeymodel.h"

namespace Qux {
namespace Model {

class KeyModelReferenceListModel : public AbstractListModel
{
    Q_OBJECT

public:
    KeyModelReferenceListModel(const AbstractKeyModel* model, QObject* parent = nullptr);

    void append(const ModelItem& key);
    void append(const QList<ModelItem>& keys);
    void clear();
    void insert(int index, const ModelItem& key);
    void insert(int index, const QList<ModelItem>& keys);
    void removeAt(int index);
    void update(int index, const ModelItem& key);

    const QList<ModelItem>& list() const override { return m_list; };

    ModelItem at(int index) const override { return m_list.at(index); }

    int count() const override { return m_list.size(); }

private:
    const AbstractKeyModel* m_model;
    QList<ModelItem> m_keyList;

    QList<ModelItem> m_list;

};

}
}

#endif