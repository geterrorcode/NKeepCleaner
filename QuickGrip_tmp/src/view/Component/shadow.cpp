#include "shadow.h"
#include "../../ui.h"
#include "../../common/tool/getroundedrectregion.h"
#include "../../common/tool/drawshadow.h"

Shadow::Shadow(QWidget* target, QWidget* parent)
    : QWidget(parent), m_distance(20), m_blurRadius(20.0), m_targetRadius(8.0), m_color(0, 0, 0, 130), m_offset(0, 0), m_drawable(true)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    {
        auto updateGeometry = [=]() {
            setGeometry(target->geometry().adjusted(-m_distance, -m_distance, m_distance, m_distance).translated(m_offset));
        };
        updateGeometry();
        connect(rcv(target), &Receiver::geometryChanged, this, updateGeometry);
        connect(rcv(target), &Receiver::isActiveWindowChanged, this, updateGeometry);    // resize bug?
        connect(this, &Shadow::distanceChanged, updateGeometry);
        connect(this, &Shadow::offsetChanged, updateGeometry);
    }

    {
        auto updateMask = [this]() {
            setMask(QRegion(rect()) -
                getRoundedRectRegion(width() - m_distance * 2, height() - m_distance * 2, m_targetRadius)
                .translated(m_distance, m_distance).translated(-m_offset));
        };
        updateMask();
        connect(this, &Shadow::distanceChanged, updateMask);
        connect(this, &Shadow::targetRadiusChanged, updateMask);
        connect(this, &Shadow::offsetChanged, updateMask);
        connect(rcv(this), &Receiver::sizeChanged, updateMask);
    }

    {
        auto updateVisible = [=]() {
            setVisible(!target->isMaximized() && target->isVisible() && Common::UI::findWindow(QApplication::activeWindow()) == target);
        };
        updateVisible();
        connect(rcv(target), &Receiver::maximizedChanged, this, updateVisible);
        connect(rcv(target), &Receiver::visibleChanged, this, updateVisible);
        connect(qobject_cast<QApplication*>(QApplication::instance()), &QApplication::focusChanged, this, updateVisible);
    }

    connect(this, &Shadow::blurRadiusChanged, this, static_cast<void (QWidget::*)()>(&QWidget::update));
    connect(this, &Shadow::colorChanged, this, static_cast<void (QWidget::*)()>(&QWidget::update));
    connect(this, &Shadow::drawableChanged, this, static_cast<void (QWidget::*)()>(&QWidget::update));
}

void Shadow::paintEvent(QPaintEvent* event)
{
    if (m_drawable) {
        QPainter painter(this);
        drawShadow(&painter, size(), rect().adjusted(m_distance, m_distance, -m_distance, -m_distance).translated(m_offset), m_color, m_blurRadius);
    }
}