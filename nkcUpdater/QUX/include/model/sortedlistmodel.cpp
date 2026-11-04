#include "sortedlistmodel.h"
#include "../utility.h"

namespace Qux {
namespace Model {

SortedListModel::SortedListModel(const AbstractListModel* model, QObject* parent)
    : AbstractListModel(parent)
{
    auto indexList = createQValue<QList<int>>(this);

    {
        auto setupSortedList = [this, model, indexList] () {
            indexList->clear();
            if (!m_sortedList.empty()) {
                auto end = m_sortedList.size() - 1;
                m_sortedList.clear();

                emit removed(0, end);
                emit countChanged(count());
            }

            m_sortedList = model->list();

            std::fill_n(std::back_inserter(*indexList), m_sortedList.size(), 0);
            std::iota(indexList->begin(), indexList->end(), 0);

            if (m_comparer) {
                std::sort(m_sortedList.begin(), m_sortedList.end(), m_comparer);
                std::sort(indexList->begin(), indexList->end(), [=] (int a, int b) {
                    return m_comparer(model->list()[a], model->list()[b]);
                });
            }

            if (!m_sortedList.empty()) {
                emit inserted(0, m_sortedList);
                emit countChanged(count());
            }
        };
        setupSortedList();
        connect(this, &SortedListModel::comparerChanged, setupSortedList);
    }

    auto insert = [this, indexList] (int start, const QList<ModelItem>& list) {
        foreach (auto value, list) {
            auto sortedIndex =
                m_comparer ?
                std::lower_bound(m_sortedList.begin(), m_sortedList.end(), value, m_comparer)  - m_sortedList.begin() :
                start;

            *indexList = mapList(*indexList, [=] (int index) {
                return index >= sortedIndex ? index + 1 : index;
            });
            indexList->insert(start++, sortedIndex);

            m_sortedList.insert(sortedIndex, value);

            emit inserted(sortedIndex, toList(value));
        }

        emit countChanged(count());
    };
    connect(model, &AbstractListModel::inserted, this, insert);

    auto remove = [this, indexList] (int start, int end) {
        if (start == 0 && end == m_sortedList.size() - 1) {
            indexList->clear();
            m_sortedList.clear();

            emit removed(start, end);
            emit countChanged(count());
            return;
        }

        for (; start <= end; end--) {
            auto sortedIndex = indexList->at(start);

            *indexList = mapList(*indexList, [=] (int index) {
                return index > sortedIndex ? index - 1 : index;
            });
            indexList->removeAt(start);

            m_sortedList.removeAt(sortedIndex);

            emit removed(sortedIndex, sortedIndex);
        }

        emit countChanged(count());
    };
    connect(model, &AbstractListModel::removed, this, remove);

    connect(model, &AbstractListModel::updated, this, [this, indexList, insert, remove] (int start, const QList<ModelItem>& list) {
        foreach (auto value, list) {
            auto sortedIndex = indexList->at(start),
                 updatedSortedIndex = m_comparer ?
                                      std::lower_bound(m_sortedList.begin(), m_sortedList.end(), value, m_comparer) - m_sortedList.begin() :
                                      sortedIndex;

            if (sortedIndex == updatedSortedIndex || sortedIndex + 1 == updatedSortedIndex) {
                m_sortedList.replace(sortedIndex, value);

                emit updated(sortedIndex, toList(value));
            }
            else {
                remove(start, start);
                insert(start, toList(value));
            }

            start++;
        }
    });
}

}
}