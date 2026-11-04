#include "keymodelreferencelistmodel.h"
#include "../utility.h"

namespace Qux {
namespace Model {

KeyModelReferenceListModel::KeyModelReferenceListModel(const AbstractKeyModel* model, QObject* parent)
    : m_model(model), AbstractListModel(parent)
{
    auto updateList = [this] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        QMap<ModelItem, ModelItem> map;
        foreach (auto& pair, pairs) {
            map[pair.first] = pair.second;
        }

        for (auto i = 0; i < m_keyList.size(); i++) {
            const auto& key = m_keyList[i];
            if (map.contains(key)) {
                const auto& value = map[key];
                m_list[i] = value;
                emit updated(i, toList(value));
            }
        }
    };

    connect(model, &AbstractKeyModel::inserted, this, updateList);
    connect(model, &AbstractKeyModel::updated, this, updateList);
    connect(model, &AbstractKeyModel::removed, this, [updateList] (const QList<ModelItem>& keys) {
        updateList(convertList<QPair<ModelItem, ModelItem>>(keys, [] (const ModelItem& key) {
            return qMakePair(key, QVariant());
        }));
    });
}

void KeyModelReferenceListModel::append(const ModelItem& key)
{
    append(toList(key));
}

void KeyModelReferenceListModel::append(const QList<ModelItem>& keys)
{
    insert(count(), keys);
}

void KeyModelReferenceListModel::clear()
{
    if (m_list.empty()) return;

    m_keyList.clear();

    auto end = m_list.size() - 1;
    m_list.clear();

    emit removed(0, end);
    emit countChanged(count());
}

void KeyModelReferenceListModel::insert(int index, const ModelItem& key)
{
    insert(index, toList(key));
}

void KeyModelReferenceListModel::insert(int index, const QList<ModelItem>& keys)
{
    index = std::max(std::min(index, count()), 0);

    QList<ModelItem> values;

    foreach (auto& key, reverseList(keys)) {
        m_keyList.insert(index, key);

        auto value = m_model->value(key);
        m_list.insert(index, value);
        values.insert(0, value);
    }

    emit inserted(index, values);
    emit countChanged(count());
}

void KeyModelReferenceListModel::removeAt(int index)
{
    if (index < 0 || index >= count()) return;

    m_keyList.removeAt(index);
    m_list.removeAt(index);

    emit removed(index, index);
    emit countChanged(index);
}

void KeyModelReferenceListModel::update(int index, const ModelItem& key)
{
    if (index < 0 || index >= count()) return;

    m_keyList[index] = key;
    auto value = m_list[index] = m_model->value(key);;

    emit updated(index, toList(value));
}

}
}