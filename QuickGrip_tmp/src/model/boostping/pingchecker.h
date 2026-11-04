#pragma once

#include <QObject>
#include <QThread>
#include <QProcess>

#include <controller/boostping/boostpingcontroller.h>


class PingChecker : public QThread
{
	Q_OBJECT

public:
	PingChecker(QObject *parent = Q_NULLPTR);
	~PingChecker();
	void run() override;

private:
	bool _mincheck;
	void pingCheck();
	int pingExeptionControl();
	
public:
	void setMinCheck(bool min);


Q_SIGNALS:
	void sendPingSpeed(bool min, int speed);
	//void LastPingCheckEnd();

};
