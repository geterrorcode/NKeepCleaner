#pragma once

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>
#include <QDebug>

//#include <QObject>
#include "../ux/singleton.h"
//#include <common/ux/signalobject.h>

class LocalServer : public QObject, public common::ux::Singleton<LocalServer>
{
	Q_OBJECT

public:
	LocalServer(QObject *parent = Q_NULLPTR);
	~LocalServer();

	void setServer();

Q_SIGNALS:
	void getup();
};
