#ifndef QUX_UI_RECEIVER_H
#define QUX_UI_RECEIVER_H

#include "../defineproperty.h"

#include <QWidget>

namespace Qux {
namespace UI {

class Receiver : public QObject
{
    Q_OBJECT

    PROPERTY_READONLY(bool, hovered, setHovered)
    PROPERTY_READONLY(bool, pressed, setPressed)

public:
    Receiver(QWidget* widget);

private:
    bool eventFilter(QObject* obj, QEvent* event) override;

    QPoint m_pressedPos;

signals:
    void changed(QEvent* event);
    void clicked(QMouseEvent* event);
    void closed(QCloseEvent* event, bool* consumed);
    void dragEntered(QDragEnterEvent* event);
    void dragged(QMouseEvent* event);
    void dragLeaved(QDragLeaveEvent* event);
    void dragMoved(QDragMoveEvent* event);
    void dropped(QDropEvent* event);
    void entered(QEvent* event);
    void focusIn(QFocusEvent* event);
    void focusOut(QFocusEvent* event);
    void hidden(QHideEvent* event);
    void keyPressed(QKeyEvent* event, bool* consumed);
    void keyReleased(QKeyEvent* event, bool* consumed);
    void leaved(QEvent* event);
    void mouseDoubleClicked(QMouseEvent* event, bool* consumed);
    void mouseMoved(QMouseEvent* event, bool* consumed);
    void mousePressed(QMouseEvent* event, bool* consumed);
    void mouseReleased(QMouseEvent* event, bool* consumed);
    void moved(QMoveEvent* event);
    void resized(QResizeEvent* event);
    void showed(QShowEvent* event);

    void enabledChanged(bool enabled);
    void focusChanged(bool focus);
    void geometryChanged(const QRect& geometry);
    void heightChanged(int height);
    void isActiveWindowChanged(bool isActiveWindow);
    void maximizedChanged(bool maximized);
    void minimizedChanged(bool minimized);
    void posChanged(const QPoint& pos);
    void sizeChanged(const QSize& size);
    void visibleChanged(bool visible);
    void widthChanged(int width);
    void windowStateChanged(Qt::WindowStates windowState);
    void xChanged(int x);
    void yChanged(int y);
};

}}

#endif
