#include "keymodel.h"
#include "../utility.h"

namespace Qux {
namespace Model {

KeyModel::KeyModel(QObject* parent)
    : AbstractKeyModel(parent)
{}

void KeyModel::clear()
{
    if (m_map.empty()) return;

    auto keys = m_map.keys();
    m_map.clear();

    emit removed(keys);
    emit countChanged(count());
}

void KeyModel::insert(const QList<QPair<ModelItem, ModelItem>>& pairs)
{
    QList<QPair<ModelItem, ModelItem>> insertedPairs, updatedPairs;
    foreach (auto& pair, pairs) {
        auto& key = pair.first;
        auto& value = pair.second;

        if (!m_map.contains(key)) insertedPairs.append(pair);
        else updatedPairs.append(pair);

        m_map.insert(key, value);
    }

    if (!insertedPairs.empty()) {
        emit inserted(insertedPairs);
        emit countChanged(count());
    }

    if (!updatedPairs.empty()) emit updated(updatedPairs);
}

void KeyModel::insert(const ModelItem& key, const ModelItem& value)
{
    insert(toList(qMakePair(key, value)));
}

void KeyModel::remove(const ModelItem& key)
{
    if (m_map.remove(key) > 0) {
        emit removed(toList(key));
        emit countChanged(count());
    }
}
}
}