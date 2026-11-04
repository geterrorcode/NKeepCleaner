#include "ui.h"

#include <QtWidgets>

#include "../QuickGrip_tmp/src/view/Component/containerlayout.h"

//
//QLayout* wrapWidget(QWidget* widget, int left, int top, int right, int bottom, Qt::Alignment alignment)
//{
//    auto layout = new ContainerLayout;
//    layout->setContentsMargins(left, top, right, bottom);
//    layout->addWidget(widget, alignment);
//    return layout;
//}

QWidget* findWindow(QWidget* widget)
{
    auto w = widget;
    for (; w && (w->windowFlags() & Qt::WindowType_Mask) != Qt::Window && w->parentWidget() != nullptr; w = w->parentWidget()->window());
    return w;
}

bool isInHierarchy(const QWidget* parent, const QWidget* child)
{
    auto c = child;
    for (; c && c != parent && c->parentWidget() != nullptr; c = c->parentWidget());
    return parent == c;
}
