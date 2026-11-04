#pragma once

//#include "../network/asyncimage.h"


#include "../../common/defineproperty/defineproperty.h"
#include <QWidget>

class ImageView : public QWidget
{
    Q_OBJECT

   
    PROPERTY(bool, circular, setCircular)
    PROPERTY(qreal, topRadius, setTopRadius)
    PROPERTY(qreal, radius, setRadius)
    //PROPERTY_NOCHANGE(AsyncImage*, image)

public:
    ImageView(QWidget* parent = nullptr);
    

protected:
    void paintEvent(QPaintEvent* event) override;

};





