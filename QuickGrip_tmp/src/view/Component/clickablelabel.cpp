#include "clickablelabel.h"

#include "../../common/defines/ssdefins.h"
#include "../../ui.h"

//ClickableLabel::ClickableLabel(QObject *parent)
//	: QObject(parent)
//{}



ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    //setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);
    this->setCursor(Qt::PointingHandCursor);

    setWordWrap(true);

    setStyleSheet("QLabel {padding-bottom: 1px; font: bold normal 16px; color: #FFFFFF; text-decoration: underline; }");

    //setStyleSheet(QString(m_bold ? ssdefines::customBoldlabel : ssdefines::customlabel)
    //    .arg(QString("%1px").arg(m_fontSize)
    //        , m_color
    //        , m_border == 0 ? QString("none") : QString("%1px").arg(m_border)));

}

ClickableLabel::~ClickableLabel()
{}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }

    QWidget::mousePressEvent(event);
}

//
//void ClickableLabel::mousePressEvent(QMouseEvent* event) {
//    emit clicked();
//}