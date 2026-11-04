#pragma once

#include <windows.h>
#include <QObject>
#include <QDebug>

#include <QAbstractNativeEventFilter>

#include "../../../QuickGrip_tmp.h"

class WndEventFilter  : public QObject, public QAbstractNativeEventFilter
{
	Q_OBJECT

public:
	WndEventFilter(QObject *parent = Q_NULLPTR);
	~WndEventFilter();

public:
	virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) Q_DECL_OVERRIDE;

Q_SIGNALS:
	void sendWindowShow();
};
