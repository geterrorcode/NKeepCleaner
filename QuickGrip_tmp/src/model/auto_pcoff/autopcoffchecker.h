#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>

class AutoPcOffChecker : public QThread
{
	Q_OBJECT

public:
	AutoPcOffChecker(QObject *parent = Q_NULLPTR);
	~AutoPcOffChecker();


public:
	void run() override;

private:
	QMutex mutex;

	bool checkSchedule();
};
