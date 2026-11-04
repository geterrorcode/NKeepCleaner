#include "makemodal.h"
#include "../ui.h"

void makeModal(QWidget* widget)
{
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    widget->setWindowModality(Qt::WindowModal);

    QObject::connect(rcv(widget), &Receiver::showed, [=] () {
        widget->move(widget->parentWidget()->geometry().center() - widget->rect().bottomRight() / 2);
    });
}