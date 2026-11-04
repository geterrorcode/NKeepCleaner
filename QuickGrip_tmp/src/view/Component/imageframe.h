#pragma once

#include "../../common/defineproperty/defineproperty.h"

#include <QFrame>

class ImageFrame : public QFrame
{
    Q_OBJECT

    PROPERTY(QString, resource, setResource)

public:
    ImageFrame(int width, int height, QString resource, QWidget* parent = nullptr);


};
