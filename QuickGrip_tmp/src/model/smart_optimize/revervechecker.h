#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>

class ReverveChecker  : public QThread
{
	Q_OBJECT

public:
	ReverveChecker(QObject *parent = Q_NULLPTR);
	~ReverveChecker();


	void run() override;

private:
	QMutex mutex;

	bool checkReserve();


Q_SIGNALS:
	void checkOptimizeReserve();

};

