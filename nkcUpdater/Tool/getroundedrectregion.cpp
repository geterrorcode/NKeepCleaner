#include "getroundedrectregion.h"

QRegion getRoundedRectRegion(int width, int height, int radius)
{
    QRegion region;

    radius -= 1;
    for (auto i = 0; i <= radius; i++) {
        auto j = radius - i;
        region += QRegion(i, j, width - i * 2, height - j * 2);
    }
    
    return region;
}