#include "convertedkeymodel.h"

namespace Qux {
namespace Model {

ConvertedKeyModel::ConvertedKeyModel(const AbstractKeyModel* model, QObject* parent)
    : AbstractKeyModel(parent)
{
    {
        auto setupConvertedMap = [this, model] () {
            if (!m_convertedMap.empty()) {
                auto keys = m_convertedMap.keys();
                m_convertedMap.clear();

                emit removed(keys);
                emit countChanged(count());
            }

            QList<QPair<ModelItem, ModelItem>> pairs;
            for (auto iter = model->map().begin(); iter != model->map().end(); iter++) {
                auto& key = iter.key();
                auto value = m_converter ? m_converter(iter.value()) : iter.value();
                m_convertedMap.insert(key, value);
                pairs.append(qMakePair(key, value));
            }

            if (!pairs.empty()) {
                emit inserted(pairs);
                emit countChanged(count());
            }
        };
        setupConvertedMap();
        connect(this, &ConvertedKeyModel::converterChanged, setupConvertedMap);
    }

    connect(model, &AbstractKeyModel::inserted, this, [this] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        QList<QPair<ModelItem, ModelItem>> convertedPairs;
        foreach (auto& pair, pairs) {
            auto& key = pair.first;
            auto value = m_converter ? m_converter(pair.second) : pair.second;
            m_convertedMap.insert(key, value);
            convertedPairs.append(qMakePair(key, value));
        }

        emit inserted(convertedPairs);
        emit countChanged(count());
    });

    connect(model, &AbstractKeyModel::removed, this, [this] (const QList<ModelItem>& keys) {
        foreach (auto& key, keys) {
            m_convertedMap.remove(key);
        }

        emit removed(keys);
        emit countChanged(count());
    });

    connect(model, &AbstractKeyModel::updated, this, [this] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
        QList<QPair<ModelItem, ModelItem>> updatedPairs;
        foreach (auto& pair, pairs) {
            auto& key = pair.first;
            auto value = m_converter ? m_converter(pair.second) : pair.second;

            if (value != m_convertedMap[key]) {
                m_convertedMap.insert(key, value);
                updatedPairs.append(qMakePair(key, value));
            }
        }

        if (!updatedPairs.empty()) emit updated(updatedPairs);
    });
}

}
}