#pragma once

#include <QPushButton>
#include <QEvent>

//class balloontooltip;
class Button : public QPushButton
{
    Q_OBJECT

public:
    Button(QWidget* parent = nullptr);

    bool _checked;

    //void setBalloonTooltip(int fontSize, QString tooltipText);

};
