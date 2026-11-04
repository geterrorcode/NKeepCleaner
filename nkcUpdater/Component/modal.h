#pragma once

#include <QWidget>

class Modal : public QWidget
{
    Q_OBJECT

public:
    Modal(/*QString header, */QString body, QString ok, QString cancel, QWidget* parent = nullptr);

signals:
    void okClicked();
    void cancelClicked();

};
