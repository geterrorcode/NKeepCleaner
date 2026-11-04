#include "filteredkeymodel.h"

namespace Qux {
namespace Model {

FilteredKeyModel::FilteredKeyModel(const AbstractKeyModel* model, QObject* parent)
    : AbstractKeyModel(parent)
{
    {
        auto setupFilteredMap = [this, model] () {
            if (!m_filteredMap.empty()) {
                auto keys = m_filteredMap.keys();
                m_filteredMap.clear();

                emit removed(keys);
                emit countChanged(count());
            }

            QList<QPair<ModelItem, ModelItem>> pairs;
            for (auto iter = model->map().begin(); iter != model->map().end(); iter++) {
                auto& key = iter.key();
                auto& value = iter.value();
                if (!m_filter || m_filter(value)) {
                    m_filteredMap.insert(key, value);
                    pairs.append(qMakePair(key, value));
                }
            }

            if (!pairs.empty()) {
                emit inserted(pairs);
                emit countChanged(count());
            }
        };
        setupFilteredMap();
        connect(this, &FilteredKeyModel::filterChanged, setupFilteredMap);
    }

    connect(model, &AbstractKeyModel::inserted, this, [this] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        QList<QPair<ModelItem, ModelItem>> insertedPairs;
        foreach (auto& pair, pairs) {
            auto& key = pair.first;
            auto& value = pair.second;

            if (!m_filter || m_filter(value)) {
                m_filteredMap.insert(key, value);
                insertedPairs.append(pair);
            }
        }

        if (!insertedPairs.empty()) {
            emit inserted(insertedPairs);
            emit countChanged(count());
        }
    });

    connect(model, &AbstractKeyModel::removed, this, [this] (const QList<ModelItem>& keys) {
        QList<ModelItem> removedKeys;
        foreach (auto& key, keys) {
            if (m_filteredMap.contains(key)) {
                m_filteredMap.remove(key);
                removedKeys.append(key);
            }
        }

        if (!removedKeys.empty()) {
            emit removed(removedKeys);
            emit countChanged(count());
        }
    });

    connect(model, &AbstractKeyModel::updated, this, [this] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        auto oldCount = count();
        QList<QPair<ModelItem, ModelItem>> insertedPairs;
        QList<QPair<ModelItem, ModelItem>> updatedPairs;
        QList<ModelItem> removedKeys;

        foreach (auto& pair, pairs) {
            auto& key = pair.first;
            auto& value = pair.second;

            auto visible = m_filteredMap.contains(key),
                 updatedVisible = !m_filter || m_filter(value);

            if (!visible && updatedVisible) {
                m_filteredMap.insert(key, value);
                insertedPairs.append(pair);
            }
            else if (visible && !updatedVisible) {
                m_filteredMap.remove(key);
                removedKeys.append(key);
            }
            else if (visible && updatedVisible) {
                m_filteredMap.insert(key, value);
                updatedPairs.append(pair);
            }
        }

        if (!insertedPairs.empty()) emit inserted(insertedPairs);
        if (!updatedPairs.empty()) emit updated(updatedPairs);
        if (!removedKeys.empty()) emit removed(removedKeys);

        if (oldCount != count()) emit countChanged(count());
    });
}

}
}