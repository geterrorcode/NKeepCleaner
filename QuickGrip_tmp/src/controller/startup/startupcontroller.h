#pragma once

#include <QObject>
#include <QMap>
#include <QList>

#include "../../common/ux/singleton.h"

#include "../../model/startup/startupdata.h"

class StartupController  : public QObject ,public common::ux::Singleton<StartupController>
{
	Q_OBJECT

public:
	//friend StartupController& common::ux::Singleton<StartupController>::inst();

	StartupController(QObject *parent = Q_NULLPTR);
	~StartupController();



signals:
	//void signalsetStartUpData(QMap<int, StartUpData>::iterator h, QMap<int, StartUpData>::iterator e);
	void signalsetStartDelComplete();
	void signalsetStartUpData(QVector<StartUpData>::iterator h, QVector<StartUpData>::iterator e);

	

public  slots:
	void getStartupData();
	void slotgetStartupData();

	void slotdelStartup(QList<int> keys);
};

//inline StartupController& startupController() { return StartupController::inst(); }
