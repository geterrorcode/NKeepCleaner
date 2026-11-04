#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QTimer>
#include <common/qvalue.h>

#include <view/common/traypopup.h>

#include <common/defines/defineproperty.h>

class TrayPopupComponent  : public QWidget
{
	Q_OBJECT

public:
	TrayPopupComponent(QWidget *parent = Q_NULLPTR);
	~TrayPopupComponent();
};
