#include "movetrayarea.h"

#include <QApplication>
#include <QScreen>
#include <QGraphicsOpacityEffect>

#include "getscreengeometry.h"

#include "../../ui.h"

void moveTrayArea(QPoint pos, QWidget* popup)
{
    const int Margin = 25;
    auto screenGeometry = getScreenGeometry(pos);
    screenGeometry.right();


    pos = QPoint(screenGeometry.right() - popup->width() - Margin,
        screenGeometry.height() - popup->height() - Margin );
                
   
    QObject::connect(rcv(popup), &Receiver::showed, [=]() {
        popup->move(pos);
    });
}

void moveTrayArea(QWidget* popup)
{
    moveTrayArea(QCursor::pos(), popup);
}