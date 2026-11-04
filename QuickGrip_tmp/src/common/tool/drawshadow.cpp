#include "drawshadow.h"

Q_DECL_IMPORT void qt_blurImage(QPainter* p, QImage& blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);

void drawShadow(QPainter* painter, QSize size, QRect targetRect, QColor color, qreal blurRadius)
{
    QImage temp(size, QImage::Format_ARGB32_Premultiplied);
    temp.fill(0);

    QPainter(&temp).fillRect(targetRect, color);
    qt_blurImage(painter, temp, blurRadius, false, true);
}