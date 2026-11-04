#include "dragger.h"
#include "../ui.h"

Dragger::Dragger(QWidget* target, QWidget* parent)
    : QWidget(parent)
{
    auto lastMousePos = Qux::createQValue<QPoint>(this);

    connect(rcv(this), &Receiver::mouseMoved, [=] (QMouseEvent* event) {
        if (event->buttons() != Qt::LeftButton || target->isMaximized()) return;

        auto newMousePos = event->globalPos();
        target->move(target->pos() + newMousePos - *lastMousePos);

        *lastMousePos = newMousePos;
    });

    connect(rcv(this), &Receiver::mousePressed, [=] (QMouseEvent* event) {
        *lastMousePos = event->globalPos();
    });
}