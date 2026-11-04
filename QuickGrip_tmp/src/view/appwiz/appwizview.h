#pragma once

#include <QWidget>
#include <QTableWidget>

#include "../../model/appwiz/appwizdata.h"

class AppwizView  : public QWidget
{
	Q_OBJECT

public:
	AppwizView(QWidget *parent);
	~AppwizView();

public:
	void setListupdate();

private:
	QTableWidget* _table;

signals:
	void signalgetAppwizData();
	//void signalViewChange(viewIndex);
	void signaldelAppwizData(int);

public slots:
	void slotupdateAppwizList(QMap<int, AppWizData*>::iterator h, QMap<int, AppWizData*>::iterator e);
	void slotdellResult(int key);

};
