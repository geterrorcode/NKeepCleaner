#include "containerlayout.h"

ContainerLayout::ContainerLayout(QWidget* parent)
    : QLayout(parent)
{
    setContentsMargins(0, 0, 0, 0);
}

ContainerLayout::~ContainerLayout()
{
    while (auto item = takeAt(0)) {
        delete item;
    }
}

void ContainerLayout::addLayout(QLayout* layout, Qt::Alignment alignment)
{
    addChildLayout(layout);
    addItem(layout);
    setAlignment(layout, alignment);
}

void ContainerLayout::addWidget(QWidget* widget, Qt::Alignment alignment)
{
    QLayout::addWidget(widget);
    setAlignment(widget, alignment);
}

void ContainerLayout::setAlignLayout(QLayout* layout, Qt::Alignment alignment)
{
    setAlignment(layout, alignment);
}

void ContainerLayout::setAlignWidget(QWidget* widget, Qt::Alignment alignment)
{
    setAlignment(widget, alignment);
}

void ContainerLayout::addItem(QLayoutItem* item)
{
    m_list.append(item);
}

QSize ContainerLayout::sizeHint() const
{
    QString name;
    QSize size;
    for (auto item : m_list) {
        size = QSize(
            qMax(size.width(), item->sizeHint().width()),
            qMax(size.height(), item->sizeHint().height())
        );
    }

    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    size += QSize(left + right, top + bottom);

    return size;
}

QSize ContainerLayout::minimumSize() const
{
    return sizeHint();
}

int ContainerLayout::count() const
{
    return m_list.size();
}

QLayoutItem* ContainerLayout::itemAt(int idx) const
{
    return m_list.value(idx);
}

QLayoutItem* ContainerLayout::takeAt(int idx)
{
    return idx >= 0 && idx < m_list.size() ?
           m_list.takeAt(idx) :
           nullptr;
}

void ContainerLayout::setGeometry(const QRect& r)
{
    QLayout::setGeometry(r);

    if (m_list.isEmpty()) {
        return;
    }

    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    for (auto& item : m_list) {
        item->setGeometry(r.adjusted(left, top, -right, -bottom));
    }
}