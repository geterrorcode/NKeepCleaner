#include "getscreengeometry.h"

#include <QApplication>
#include <QScreen>

QRect getScreenGeometry(QPoint pos)
{
    QRect geometry;
    for (auto screen : QApplication::screens()) {
        if (screen->geometry().contains(pos)) {
            geometry = screen->availableGeometry();
            break;
        }
    }
    return geometry;
}