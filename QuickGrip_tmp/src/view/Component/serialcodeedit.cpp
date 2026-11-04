#include "serialcodeedit.h"

#include <QApplication>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>


SerialCodeEdit::SerialCodeEdit(QWidget* parent)
	: QLineEdit(parent)
{
    this->setMaxLength(14);

    connect(this, &QLineEdit::textChanged, this, &SerialCodeEdit::onTextChanged);
}

SerialCodeEdit::~SerialCodeEdit()
{}

QString _last_string = "";
void SerialCodeEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);

    if (event->key() == Qt::Key_Backspace) 
        return;

    /*if (text().length() % 5 == 4 && event->text().length() == 1) {
        
        QString nowstring = text().at(text().length() - 1);
        if (_last_string  != '-') {
            setText(text().append("-"));
        }
            
        _last_string = nowstring;
    }*/
}

void SerialCodeEdit::onTextChanged(const QString& text)
{
    QString upperText = text.toUpper();
    if (text != upperText) {
        setText(upperText);
    }
}