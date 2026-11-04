#pragma once

#include <QWidget>

class Dragger : public QWidget
{
    Q_OBJECT

public:
    Dragger(QWidget* target, QWidget* parent = nullptr);

};
