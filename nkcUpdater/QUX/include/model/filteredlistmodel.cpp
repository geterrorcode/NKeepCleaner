#include "filteredlistmodel.h"
#include "../utility.h"

namespace Qux {
namespace Model {

FilteredListModel::FilteredListModel(const AbstractListModel* model, QObject* parent)
    : AbstractListModel(parent)
{
    auto visibleList = createQValue<QList<bool>>(this);

    {
        auto setupFilteredList = [this, model, visibleList] () {
            visibleList->clear();
            if (!m_filteredList.empty()) {
                auto end = m_filteredList.size() - 1;
                m_filteredList.clear();

                emit removed(0, end);
                emit countChanged(count());
            }

            foreach (auto value, model->list()) {
                auto visible = !m_filter || m_filter(value);

                visibleList->append(visible);
                if (visible) {
                    m_filteredList.append(value);
                }
            }

            if (!m_filteredList.empty()) {
                emit inserted(0, m_filteredList);
                emit countChanged(count());
            }
        };
        setupFilteredList();
        connect(this, &FilteredListModel::filterChanged, setupFilteredList);
    }

    connect(model, &AbstractListModel::inserted, this, [this, visibleList] (int start, const QList<ModelItem>& list) {
        auto filteredStart = std::count_if(visibleList->begin(), visibleList->begin() + start, [] (bool visible) { return visible; });

        QList<ModelItem> filteredList;
        foreach (auto value, list) {
            auto visible = !m_filter || m_filter(value);

            visibleList->insert(start++, visible);

            if (visible) {
                filteredList.append(value);
            }
        }

        if (!filteredList.empty()) {
            std::copy(filteredList.begin(), filteredList.end(), std::inserter(m_filteredList, m_filteredList.begin() + filteredStart));

            emit inserted(filteredStart, filteredList);
            emit countChanged(count());
        }
    });

    connect(model, &AbstractListModel::removed, this, [this, visibleList] (int start, int end) {
        auto filteredCount = std::count_if(visibleList->begin() + start, visibleList->begin() + end + 1, [] (bool visible) { return visible; });
        visibleList->erase(visibleList->begin() + start, visibleList->begin() + end + 1);

        if (filteredCount == 0) return;

        auto filteredStart = std::count_if(visibleList->begin(), visibleList->begin() + start, [] (bool visible) { return visible; });
        auto filteredEnd = filteredStart + filteredCount - 1;

        m_filteredList.erase(m_filteredList.begin() + filteredStart, m_filteredList.begin() + filteredEnd + 1);

        emit removed(filteredStart, filteredEnd);
        emit countChanged(count());
    });

    connect(model, &AbstractListModel::updated, this, [this, visibleList] (int start, const QList<ModelItem>& list) {
        auto oldCount = count();
        auto filteredStart = std::count_if(visibleList->begin(), visibleList->begin() + start, [] (bool visible) { return visible; });

        foreach (auto value, list) {
            auto visible = bool(visibleList->at(start)),
                 updatedVisible = !m_filter || m_filter(value);

            if (!visible && updatedVisible) {
                visibleList->replace(start, true);

                m_filteredList.insert(m_filteredList.begin() + filteredStart, value);

                emit inserted(filteredStart, toList(value));
            }
            else if (visible && !updatedVisible) {
                visibleList->replace(start, false);

                m_filteredList.removeAt(filteredStart);

                emit removed(filteredStart, filteredStart);
            }
            else if (visible && updatedVisible) {
                m_filteredList.replace(filteredStart, value);

                emit updated(filteredStart, toList(value));
            }

            if (updatedVisible) filteredStart++;
            start++;
        }

        if (oldCount != count()) emit countChanged(count());
    });
}

}
}