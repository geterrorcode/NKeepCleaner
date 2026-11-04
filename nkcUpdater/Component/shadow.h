#pragma once

#include <QWidget>
#include <Qux/include/defineproperty.h>

class Shadow : public QWidget
{
    Q_OBJECT

    PROPERTY(int, distance, setDistance)
    PROPERTY(qreal, blurRadius, setBlurRadius)
    PROPERTY(qreal, targetRadius, setTargetRadius)
    PROPERTY(QColor, color, setColor)
    PROPERTY(QPoint, offset, setOffset)
    PROPERTY(bool, drawable, setDrawable)

public:
    Shadow(QWidget* target, QWidget* parent = nullptr);

private:
    void paintEvent(QPaintEvent* event) override;

};
