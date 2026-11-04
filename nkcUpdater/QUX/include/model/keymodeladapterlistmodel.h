#ifndef QUX_MODEL_KEYMODELADAPTERLISTMODEL_H
#define QUX_MODEL_KEYMODELADAPTERLISTMODEL_H

#include "abstractlistmodel.h"
#include "abstractkeymodel.h"

namespace Qux {
namespace Model {

class KeyModelAdapterListModel : public AbstractListModel
{
    Q_OBJECT

public:
    KeyModelAdapterListModel(const AbstractKeyModel* model, QObject* parent = nullptr);

    const QList<ModelItem>& list() const override { return m_list; };

    ModelItem at(int index) const override { return m_list.at(index); }

    int count() const override { return m_list.size(); }

private:
    QList<ModelItem> m_list;

};

}
}

#endif