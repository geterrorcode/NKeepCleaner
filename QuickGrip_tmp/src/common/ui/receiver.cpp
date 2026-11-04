#include "receiver.h"

#include <QWindowStateChangeEvent>
#include <QApplication>

namespace Common {
namespace UI {

Receiver::Receiver(QWidget* widget) 
    : m_hovered(false), 
      m_pressed(false),
      QObject(widget)
{
    widget->installEventFilter(this);
}

bool Receiver::eventFilter(QObject* obj, QEvent* event)
{
    auto widget = qobject_cast<QWidget*>(obj);

    switch (event->type()) {
    case QEvent::ActivationChange:
        emit changed(event);
        emit isActiveWindowChanged(widget->isActiveWindow());
        break;    
    case QEvent::Close: {
        bool consumed = false;
        emit closed(static_cast<QCloseEvent*>(event), &consumed);

        if (consumed) return true;   
        break;
    }
    case QEvent::DragEnter:
        emit dragEntered(static_cast<QDragEnterEvent*>(event));
        break;
    case QEvent::DragLeave:
        emit dragLeaved(static_cast<QDragLeaveEvent*>(event));
        break;
    case QEvent::DragMove:
        emit dragMoved(static_cast<QDragMoveEvent*>(event));
        break;
    case QEvent::Drop:
        emit dropped(static_cast<QDropEvent*>(event));
        break;
    case QEvent::EnabledChange:
        emit enabledChanged(widget->isEnabled());
        break;
    case QEvent::Enter:     
        setHovered(true);
        emit entered(event);        
        break;
    case QEvent::FocusIn:
        emit focusIn(static_cast<QFocusEvent*>(event));
        emit focusChanged(true);
        break;
    case QEvent::FocusOut:
        emit focusOut(static_cast<QFocusEvent*>(event));
        emit focusChanged(false);
        break;
    case QEvent::Hide:
        emit hidden(static_cast<QHideEvent*>(event));
        if (!event->spontaneous()) emit visibleChanged(false);
        break;
    case QEvent::KeyPress: {
        bool consumed = false;
        emit keyPressed(static_cast<QKeyEvent*>(event), &consumed);

        if (consumed) return true;          
        break;
    }
    case QEvent::KeyRelease: {
        bool consumed = false;
        emit keyReleased(static_cast<QKeyEvent*>(event), &consumed);

        if (consumed) return true;          
        break;
    }
    case QEvent::Leave:            
        emit leaved(event);
        setHovered(false);
        break;
    case QEvent::MouseButtonDblClick: {
        bool consumed = false;
        emit mouseDoubleClicked(static_cast<QMouseEvent*>(event), &consumed);

        if (consumed) return true;
        break;
    }
    case QEvent::MouseMove: {        
        auto mouseEvent = static_cast<QMouseEvent*>(event);        

        bool consumed = false;
        emit mouseMoved(mouseEvent, &consumed);

        if (consumed) return true;          

        if ((m_pressedPos - mouseEvent->globalPos()).manhattanLength() > 
            QApplication::startDragDistance()) emit dragged(mouseEvent);        
        break;
    }
    case QEvent::MouseButtonPress: {
        auto mouseEvent = static_cast<QMouseEvent*>(event);                

        bool consumed = false;
        emit mousePressed(mouseEvent, &consumed);

        if (consumed) return true;

        setPressed(true);

        m_pressedPos = mouseEvent->globalPos();        
        break;
    }
    case QEvent::MouseButtonRelease: {
        auto mouseEvent = static_cast<QMouseEvent*>(event);            

        bool consumed = false;
        emit mouseReleased(mouseEvent, &consumed);
        
        if (consumed) return true;

        setPressed(false);

        if (mouseEvent->button() == Qt::LeftButton &&
            widget->rect().contains(widget->mapFromGlobal(mouseEvent->globalPos()))) {
            emit clicked(mouseEvent);
        }        
        break;
    }
    case QEvent::Move: {
        auto moveEvent = static_cast<QMoveEvent*>(event);

        emit moved(moveEvent);
        emit posChanged(widget->pos());
        emit geometryChanged(widget->geometry());

        auto oldPos = moveEvent->oldPos();
        if (oldPos.x() != widget->x()) emit xChanged(widget->x());
        if (oldPos.y() != widget->y()) emit yChanged(widget->y());  

        break;
    }
    case QEvent::Resize: {
        auto resizeEvent = static_cast<QResizeEvent*>(event);

        emit resized(resizeEvent);
        emit sizeChanged(widget->size());
        emit geometryChanged(widget->geometry());

        auto oldSize = resizeEvent->oldSize();
        if (oldSize.width() != widget->width()) emit widthChanged(widget->width());
        if (oldSize.height() != widget->height()) emit heightChanged(widget->height());   

        break;
    }
    case QEvent::Show:
        emit showed(static_cast<QShowEvent*>(event));
        if (!event->spontaneous()) emit visibleChanged(true);
        break;
    case QEvent::WindowStateChange: {
        emit changed(event);
        emit windowStateChanged(widget->windowState());

        auto oldState = static_cast<QWindowStateChangeEvent*>(event)->oldState();
        if (bool(oldState & Qt::WindowMaximized) != widget->isMaximized()) emit maximizedChanged(widget->isMaximized());
        if (bool(oldState & Qt::WindowMinimized) != widget->isMinimized()) emit minimizedChanged(widget->isMinimized());        

        break;
    }}

    return QObject::eventFilter(obj, event);
}

}}