#pragma once

#include <QObject>
#include <QMap>
#include <QProcess>

#include "../../common/ux/singleton.h"
#include "../../model/appwiz/appwizdata.h"

class AppwizController  : public QObject ,public common::ux::Singleton<AppwizController>
{
	Q_OBJECT

public:
	AppwizController(QObject *parent = Q_NULLPTR);
	~AppwizController();

	void appwizAlertCaller();

signals:
	void signalsetAppwizData(QMap<int, AppWizData*>::iterator h, QMap<int, AppWizData*>::iterator e);
	void signaldelAppwizdataResult(int);
	void signalAppwizAlertCall();

public  slots:
	//void getStartupData();
	void slotgetAppwizData();
	void slotdelAppwizData(int key);
	void slotdelResult(int key);
};

