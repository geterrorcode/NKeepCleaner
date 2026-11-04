#include "keymodeladapterlistmodel.h"
#include "../utility.h"

namespace Qux {
namespace Model {

KeyModelAdapterListModel::KeyModelAdapterListModel(const AbstractKeyModel* model, QObject* parent)
    : AbstractListModel(parent)
{
    auto indexMap = createQValue<QMap<ModelItem, int>>(this);

    {
        // initialize list
        for (auto iter = model->map().begin(); iter != model->map().end(); iter++) {
            indexMap->insert(iter.key(), m_list.size());
            m_list.append(iter.value());
        }
    }

    connect(model, &AbstractKeyModel::inserted, this, [this, indexMap] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        QList<ModelItem> list;
        auto end = m_list.size(), i = end;
        foreach (auto& pair, pairs) {
            auto& key = pair.first;
            auto& value = pair.second;
            indexMap->insert(key, i++);
            list.append(value);
        }

        m_list.append(list);

        emit inserted(end, list);
        emit countChanged(count());
    });

    connect(model, &AbstractKeyModel::removed, this, [this, indexMap] (const QList<ModelItem>& keys) {
        if (keys.size() == m_list.size()) {
            auto end = m_list.size() - 1;
            m_list.clear();

            emit removed(0, end);
            emit countChanged(count());
            return;
        }

        foreach (auto& key, keys) {
            auto index = indexMap->take(key);

            for (auto iter = indexMap->begin(); iter != indexMap->end(); iter++) {
                auto& i = iter.value();
                i = i > index ? i - 1 : i;
            }

            m_list.removeAt(index);

            emit removed(index, index);
            emit countChanged(count());
        }
    });

    connect(model, &AbstractKeyModel::updated, this, [this, indexMap] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        foreach (auto& pair, pairs) {
            auto& key = pair.first;
            auto& value = pair.second;
            auto index = indexMap->value(key);
            m_list[index] = value;

            emit updated(index, toList(value));
        }
    });
}

}
}