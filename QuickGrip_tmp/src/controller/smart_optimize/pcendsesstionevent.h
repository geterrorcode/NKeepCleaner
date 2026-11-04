#pragma once


#include <QObject>
#include <QCoreApplication>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <Windows.h>

#include "../../../QuickGrip_tmp.h"

class PcEndSesstionEvent: public QObject, public QAbstractNativeEventFilter
{
	
	Q_OBJECT

public:
	explicit PcEndSesstionEvent(QObject* parent = nullptr)
       : QObject(parent)
   {
       if (!SetProcessShutdownParameters(0x100, SHUTDOWN_NORETRY))
       {
           qWarning() << "Failed to set process shutdown parameters.";
       }
   }
	//explicit  PcEndSesstionEvent(QObject* receiver = nullptr);
	//~PcEndSesstionEvent();

public:
	virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) Q_DECL_OVERRIDE;

private:
	QObject* _receiver;



};
