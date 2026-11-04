#include "imageframe.h"
#include "../ui.h"

ImageFrame::ImageFrame(int width, int height, QString resource, QWidget* parent)
    : QFrame(parent), m_resource(resource)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);

    if (width != 0 && 
        height != 0)
    setFixedSize(width, height);

    setStyleSheet(ss("imageWidget").arg(resource));

}
