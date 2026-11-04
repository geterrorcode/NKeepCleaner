#include "../include/ui/dynamiclistview.h"
#include "../include/ui/rcv.h"
#include "../include/utility.h"

#include <QLayout>

using namespace Qux::Model;

namespace Qux {
namespace UI {

DynamicListView::DynamicListView(QWidget* parent)
    : m_model(nullptr), m_widgetHeight(0), QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);

    auto currFirst = createQValue<int>(0, this),
         currLast = createQValue<int>(0, this);

    auto setupLayout = [=] () {
        if (!m_model || !m_widgetCreator) return;

        {
            // clear layout
            QLayoutItem* item;
            while (item = layout->takeAt(0)) {
                if (item->widget()) item->widget()->deleteLater();
                delete item;
            }
        }

        auto first = *currFirst = qMax(m_visibleArea.top(), 0) / m_widgetHeight,
             last = *currLast = qMax(m_visibleArea.bottom(), 0) / m_widgetHeight;

        setMinimumHeight(m_model->count() * m_widgetHeight);
        layout->setContentsMargins(0, std::min(first * m_widgetHeight, minimumHeight()), 0, 0);

        for (auto i = first; i <= last && i < m_model->count(); i++) {
            layout->addWidget(m_widgetCreator(m_model->at(i)));
        }

        layout->addStretch(1);
    };

    connect(this, &DynamicListView::modelAboutToBeChanged, [this] () {
        if (m_model != nullptr) {
            disconnect(m_model, 0, this, 0);
        }
    });

    connect(this, &DynamicListView::modelChanged, [=] () {
        connect(m_model, &AbstractListModel::inserted, this, [=] (int start, const QList<ModelItem>& list) {
            auto first = *currFirst, last = *currLast;

            setMinimumHeight(m_model->count() * m_widgetHeight);
            layout->setContentsMargins(0, std::min(first * m_widgetHeight, minimumHeight()), 0, 0);

            if (start > last) return;

            auto insertOffset = start < first ? 0 : start - first;
            auto insertCount = qMin(last + 1, start + list.count()) - qMax(first, start);
            auto removeCount = insertCount - ((last - first) + 1 - (layout->count() - 1));

            for (auto i = 0; i < insertCount; i++) {
                auto widget = m_widgetCreator(m_model->at(first + insertOffset + i));
                layout->insertWidget(insertOffset + i, widget);
            }

            for (auto i = 0; i < removeCount; i++) {
                auto item = layout->takeAt(layout->count() - 2);
                item->widget()->deleteLater();
                delete item;
            }
        });

        connect(m_model, &AbstractListModel::removed, this, [=] (int start, int end) {
            auto first = *currFirst, last = *currLast;

            setMinimumHeight(m_model->count() * m_widgetHeight);
            layout->setContentsMargins(0, std::min(first * m_widgetHeight, minimumHeight()), 0, 0);

            if (start > last) return;

            auto removeOffset = start < first ? 0 : start - first;
            auto removeCount = qMin(last + 1, end + 1) - qMax(first, start);
            auto insertCount = removeCount;

            for (auto i = 0; i < removeCount; i++) {
                auto item = layout->takeAt(removeOffset);
                item->widget()->deleteLater();
                delete item;
            }

            for (auto i = 0; i < insertCount; i++) {
                auto index = first + layout->count() - 1;
                if (m_model->count() <= index) break;

                auto widget = m_widgetCreator(m_model->at(index));
                layout->insertWidget(layout->count() - 1, widget);
            }
        });

        connect(m_model, &AbstractListModel::updated, this, [=] (int start, const QList<ModelItem>& list) {
            auto first = *currFirst, last = *currLast;

            foreach (auto& value, list) {
                if (start < first || start > last) break;

                auto item = layout->itemAt(start - first);
                auto widget = item->widget();

                if (widget->property("data").isValid()) {
                    widget->setProperty("data", value);
                }
                else {
                    layout->removeItem(item);
                    item->widget()->deleteLater();
                    delete item;

                    layout->insertWidget(start - first, m_widgetCreator(value));
                }

                start++;
            }
        });

        setupLayout();
    });

    connect(this, &DynamicListView::widgetHeightChanged, setupLayout);
    connect(this, &DynamicListView::widgetCreatorChanged, setupLayout);

    connect(this, &DynamicListView::visibleAreaChanged, [=] () {
        if (!m_model || !m_widgetCreator) return;

        auto first = qMax(m_visibleArea.top(), 0) / m_widgetHeight,
             last = qMax(m_visibleArea.bottom(), 0) / m_widgetHeight;

        if (first == *currFirst && last == *currLast) return;

        layout->setContentsMargins(0, std::min(first * m_widgetHeight, minimumHeight()), 0, 0);

        auto inserted = 0, removed = 0;
        for (auto i = qMin(first, *currFirst); i <= qMax(last, *currLast) && i < m_model->count(); i++) {
            if (i >= *currFirst && i <= *currLast) {
                if (i >= first && i <= last)  {
                    // do nothing
                }
                else {
                    auto index = i - *currFirst + inserted - removed;
                    auto item = layout->takeAt(index);
                    item->widget()->deleteLater();
                    delete item;

                    removed++;
                }
            }
            else if (i >= first && i <= last) {
                auto widget = m_widgetCreator(m_model->at(i));
                if (i < *currFirst) {
                    layout->insertWidget(inserted, widget);
                    inserted++;
                }
                else {
                    layout->insertWidget(layout->count() - 1, widget);
                }
            }
        }

        *currFirst = first;
        *currLast = last;
    });
}

}
}