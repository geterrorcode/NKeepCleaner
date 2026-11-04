#pragma once

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>
#include <QPointer>

#include "../../model/smart_optimize/smartopimizedata.h"
#include "../../model/smart_optimize/smartoptimizeinfo.h"
#include "../../model/smart_optimize/revervechecker.h"

#include "../../common/ux/singleton.h"

class SmartOptimizeController  : public QObject, public common::ux::Singleton<SmartOptimizeController>
{
	Q_OBJECT

public:
	SmartOptimizeController(QObject *parent = Q_NULLPTR);
	~SmartOptimizeController();

	bool isReserveData() ;

	bool _usedata;

	bool getReservePcOn();
	bool getReservePcOff();

	
private:
	QPointer<ReverveChecker> _checker;
	

Q_SIGNALS:
	void signalsetOptiReserveData(QMap<int, QString> viewdata);
	void signalupdateOptiReserveData(int key, ReserveData data);
	void signaldelOptiReserveData(QList<int> list);

	void isUseData(bool use);
	void signalAutoOptimize(bool light);
	void signalPcOFFOptimize();

	//void optimizeReserveCheck();
	void optimizeReserveSetAlert(bool del);


public Q_SLOTS:
	void slotgetOptiReserveData();
	//void slotisUseData();

	void slotsetOptiReservePcOnOff(bool pcon, bool use);
	void slotsetOptiReserve(int key, ReserveData data);
	void slotdelOptiReserve(QList<int> list);
	void slotCheckPcStartOptimize();
	void slotCheckPcOffOptimize();
};
