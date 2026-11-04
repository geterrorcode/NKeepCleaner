#include "makemodal.h"
#include "../../ui.h"

void makeModal(QWidget* widget)
{
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    widget->setWindowModality(Qt::WindowModal);

    QObject::connect(rcv(widget), &Receiver::showed, [=] () {
        if (auto parent = widget->parentWidget()) {
            const QPoint center = parent->mapToGlobal(parent->rect().center());
            widget->move(center - QPoint(widget->width() / 2, widget->height() / 2));
        }
    });
}


void makeModalclose(QWidget* widget)
{
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    widget->setWindowModality(Qt::NonModal);

    QObject::connect(rcv(widget), &Receiver::showed, [=]() {
        QApplication::setActiveWindow(widget);
        widget->setFocus();
        });

    QObject::connect(qobject_cast<QApplication*>(QApplication::instance()), &QApplication::focusChanged, widget, [=](QWidget* old, QWidget* now) {
        if (now && !Common::UI::isInHierarchy(widget, now)) {
            widget->close();
        }
        });
}
