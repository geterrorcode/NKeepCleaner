#include "commonlabel.h"
#include "../ui.h"

CommonLabel::CommonLabel(int fontSize, QString color, bool bold, int border, QWidget* parent)
    : QLabel(parent), m_fontSize(fontSize), m_color(color), m_bold(bold), m_border(border)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);

    auto update = [=] () {
        setFixedHeight(m_fontSize + 4);
      

        setStyleSheet(ss(m_bold ? "boldLabel" : "label")
                            .arg(QString("%1px").arg(m_fontSize)
                            , m_color
                            , m_border == 0 ? QString("none") : QString("%1px").arg(m_border)));
    };
    connect(this, &CommonLabel::fontSizeChanged, update);
    connect(this, &CommonLabel::colorChanged, update);
    connect(this, &CommonLabel::boldChanged, update);
    connect(this, &CommonLabel::borderChanged, update); 
    update();   
}