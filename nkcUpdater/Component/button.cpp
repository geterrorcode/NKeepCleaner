#include "button.h"

//tooltip
#include <QApplication>
#include <QDebug>

//#include "balloontooltip.h"
#include "../ui.h"

Button::Button(QWidget* parent)
    : QPushButton(parent), _checked(false)
{
    setFocusPolicy(Qt::NoFocus);
    this->setCursor(Qt::PointingHandCursor);

    //if (this->isCheckable()) {
    //    if (isChecked()) {
    //        this->setDisabled(true);
    //    }
    //    else {
    //        this->setDisabled(false);
    //    }
    //}
}



//void Button::setBalloonTooltip(int fontSize, QString tooltipText)
//{
//    auto balloon = new balloontooltip(tooltipText, fontSize, this);
//
//    
//    
//    connect(rcv(this), &Receiver::entered, [=](QEvent* event) {
//        
//        balloon->move(mapToGlobal(QPoint(rect().topRight().x() + 5, 0)));
//        balloon->setShow(true);
//     });
//
//    // Mouse Leave
//    connect(rcv(this), &Receiver::leaved, [=](QEvent* event) {
//        balloon->setShow(false);
//    });
//
//    connect(rcv(this), &Receiver::clicked, [=](QMouseEvent* event) {
//        balloon->setShow(false);
//    });
//
//}



