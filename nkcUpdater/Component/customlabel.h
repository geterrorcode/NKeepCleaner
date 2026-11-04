#pragma once

//#include "../../common/defines/defineproperty.h"
#include "../define/defineproperty.h"
#include <QLabel>

class CustomLabel : public QLabel
{
    Q_OBJECT

    PROPERTY(int, fontSize, setFontSize)
    PROPERTY(QString, color, setColor)
    PROPERTY(bool, bold, setBold)
    PROPERTY(int, border, setBorder)
    PROPERTY(int, heigth, setHeigth)

public:
    CustomLabel(int fontSize, QString color, bool bold = false, int border = 0, int heigth = 0,  QWidget* parent = nullptr);

};
