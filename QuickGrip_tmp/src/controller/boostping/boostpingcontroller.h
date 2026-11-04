#pragma once

#include <QObject>

#include "../../common/ux/singleton.h"

class BoostPingController  : public QObject
	, public common::ux::Singleton<BoostPingController>
{
	Q_OBJECT

public:
	BoostPingController(QObject *parent = Q_NULLPTR);
	~BoostPingController();

private:
	int _count;
	QTimer* _timer;

	unsigned int _unuse_count;

	//int _unuse_count;

	uint _start_ping;
	uint _end_ping;

	bool _pingcheckend;
	bool _boostRunning;
	bool _alertEmitted;

	void checkPingSpeed();


public:
	void setStepCount(int step);

	bool lastpingcheck();

signals:
	void signalStatusUpdate(QString);
	void signalboostProgress(int per);
	void signalInternetOffAlert();
	void signalboostEnd();

	void signalboostReSetEnd();
	
	void signalSetBeforePingSpeed(int speed);
	void signalSetAfterPingSpeed(int speed);

	void signalLastPingCheck();

public  slots:
	void slotBoostPingStep(int step);
	void slotUnSetBoostPingStep();

	void slotBoostPing();
	void slotBoostPungOff();

	void slotNetworkOn();
	void slotNetworkOff();


	void slotBeforePingCheck();

	
};
