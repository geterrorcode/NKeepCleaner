#pragma once

#include <QLabel>
#include <Qux/include/defineproperty.h>



class CommonLabel : public QLabel
{
    Q_OBJECT

    PROPERTY(int, fontSize, setFontSize)
    PROPERTY(QString, color, setColor)
    PROPERTY(bool, bold, setBold)
    PROPERTY(int, border, setBorder)

public:
    CommonLabel(int fontSize, QString color, bool bold = false, int border = 0, QWidget* parent = nullptr);

};
