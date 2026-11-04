#pragma once

#include <QPainter>

void drawShadow(QPainter* painter, QSize size, QRect targetRect, QColor color, qreal blurRadius);
