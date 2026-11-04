#pragma once

#include <QFrame>
#include <Qux/include/defineproperty.h>

class ImageFrame : public QFrame
{
    Q_OBJECT

    PROPERTY(QString, resource, setResource)

public:
    ImageFrame(int width, int height, QString resource, QWidget* parent = nullptr);

};
