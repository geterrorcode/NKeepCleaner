#ifndef QUX_MODEL_LISTMODEL_H
#define QUX_MODEL_LISTMODEL_H

#include "abstractlistmodel.h"

namespace Qux {
namespace Model {

class ListModel : public AbstractListModel
{
    Q_OBJECT

public:
    ListModel(QObject* parent = nullptr);

    void append(const ModelItem& value);
    void append(const QList<ModelItem>& values);
    void clear();
    void insert(int index, const ModelItem& value);
    void insert(int index, const QList<ModelItem>& values);
    void removeAt(int index);
    void update(int index, const ModelItem& value);

    const QList<ModelItem>& list() const override { return m_list; };

    ModelItem at(int index) const override { return m_list.value(index); }

    int count() const override { return m_list.count(); }

private:
    QList<ModelItem> m_list;

};

}
}

#endif