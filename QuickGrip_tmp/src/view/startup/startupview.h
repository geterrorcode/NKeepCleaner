#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QMap>
#include <QList>

#include "../../model/startup/startupdata.h"
#include "../../common/defines/commondefine.h"

#include "view/Component/button.h"

#include "../../ui.h"

//#include "../../controller/startup/startupcontroller.h"


class StartUpView  : public QWidget
{
	Q_OBJECT

public:
	StartUpView(QWidget *parent);
	~StartUpView();

private: 
	QTableWidget* _table;
	Button* _delbtn;

	//QPointer<StartupController> _startup_controller;

	void getStartUp();
	void updateStartUp();

	QString typeToString(StartUpType type);

	//void delStartup(int index);
public:
	void setListupdate();


signals:
	void signalgetStartup();
	void signalViewChange(viewIndex);
	void signaldelStartup(QList<int>);

public slots:
	//void slotupdateStartupList(QMap<int, StartUpData>::iterator h, QMap<int, StartUpData>::iterator e);
	void slotupdateStartupList(QVector<StartUpData>::iterator h, QVector<StartUpData>::iterator e);
	
};
