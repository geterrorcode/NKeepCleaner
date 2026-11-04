#include "copylabel.h"
#include <QKeyEvent>



CopyLabel::CopyLabel(size_t fontsize, bool bold, QString color, QWidget *parent)
	: QTextEdit(parent)
{
	setReadOnly(true);
    
    this->setStyleSheet(QString("QTextEdit {padding-bottom: 1px; color: %1; border=none; font: %2 normal %3px; background-color: transparent;}"
        " QMenu { border-radius: 3px; border: 1px solid #cecece; color: #ffffff;}"
    " QMenu::item:selected {background-color: #333366; } ").arg(color).arg(bold ? "bold" : "nomal").arg(fontsize));

    //this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CopyLabel::~CopyLabel()
{
}


void CopyLabel::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Copy)) {
        QTextEdit::keyPressEvent(event);
    }
    else {
        // 그 외의 키 이벤트를 무시.
        event->ignore();
    }
}