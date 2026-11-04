#include "imageview.h"
#include "../../ui.h"

ImageView::ImageView(QWidget* parent)
    : QWidget(parent), m_image(new AsyncImage(this)), m_circular(false), m_radius(0.0), m_topRadius(0.0)
{
    connect(m_image, &AsyncImage::pixmapChanged, this, static_cast<void (QWidget::*)()>(&QWidget::update));

    {

        auto updateMask = [this] () {
            if (m_circular) {
                // adjust 2 pixels for antialiasing circle
                setMask(QRegion(rect().adjusted(-2, -2, 2, 2), QRegion::Ellipse));
                update();
            }
            else {
                clearMask();
            }
        };
        connect(this, &ImageView::circularChanged, updateMask);
        connect(rcv(this), &Receiver::sizeChanged, updateMask);
    }
}

void ImageView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    auto pixmap = m_image->pixmap();
    if (pixmap.isNull()) {
        QStyleOption opt;
        opt.init(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    }
    else {
        auto scaledPixmap = pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QImage img(scaledPixmap.size(), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);

        if (m_circular) {
            QPainter painter(&img);
            painter.setRenderHint(QPainter::Antialiasing);

            QPainterPath clip;
            clip.addEllipse(img.rect());
            painter.setClipPath(clip);
            painter.drawPixmap(0, 0, scaledPixmap);
        }
        else {
            QPainter painter(&img);
            painter.setRenderHint(QPainter::Antialiasing);

            if (m_radius > 0) {               
                QPainterPath clip;
                clip.addRoundedRect(img.rect(), m_radius, m_radius);
                painter.setClipPath(clip);
            }
            else if (m_topRadius > 0) {
                QPainterPath clip;
                clip.setFillRule(Qt::WindingFill);
                clip.addRoundedRect(img.rect(), m_topRadius, m_topRadius);
                clip.addRect(img.rect().adjusted(0, m_topRadius, 0, 0));
                painter.setClipPath(clip);
            }

            painter.drawPixmap(0, 0, scaledPixmap);
        }

        painter.drawImage(rect(), img);
    }
}
