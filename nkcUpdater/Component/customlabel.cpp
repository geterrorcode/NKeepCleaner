#include "CustomLabel.h"

//#include "../../common/defines/ssdefins.h"
//#include "../../QuickGrip_tmp/src/common/defines/ssdefins.h"
#include "../define/ssdefins.h"
#include "../ui.h"

CustomLabel::CustomLabel(int fontSize, QString color, bool bold, int border, int heigth, QWidget* parent)
    : QLabel(parent), m_fontSize(fontSize), m_color(color), m_bold(bold), m_border(border), m_heigth(heigth)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setWordWrap(true);

    auto update = [=] () {

        if (m_heigth != 0)
            setFixedHeight(m_heigth);
        else
            setFixedHeight(m_fontSize + 4);
      
        //QString ss = ssdefines::imageWidget.arg(":/smartoptimize/resource/smartoptimize/descript.png");
       /* QString ss = QString(m_bold ? ssdefines::customBoldlabel : ssdefines::customlabel)
            .arg(QString("%1px").arg(m_fontSize)
                , m_color
                , m_border == 0 ? QString("none") : QString("%1px").arg(m_border));*/

        //setStyleSheet(ss(m_bold ? "boldLabel" : "label")
        setStyleSheet(QString(m_bold ? ssdefines::customBoldlabel : ssdefines::customlabel)
                            .arg(QString("%1px").arg(m_fontSize)
                            , m_color
                            , m_border == 0 ? QString("none") : QString("%1px").arg(m_border)));
    };
    connect(this, &CustomLabel::fontSizeChanged, update);
    connect(this, &CustomLabel::colorChanged, update);
    connect(this, &CustomLabel::boldChanged, update);
    connect(this, &CustomLabel::borderChanged, update);
    update();   
}