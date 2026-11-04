#include "imageframe.h"
#include "../../ui.h"

ImageFrame::ImageFrame(int width, int height, QString resource, QWidget* parent)
    : QFrame(parent), m_resource(resource)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFixedSize(width, height);
    bind<QString>(REF(this, resource), BOUND(this, styleSheet), [](QString resource) {
        return resource.isEmpty() ? "" : ss("imageWidget").arg(resource);
    });
}
