#pragma once

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPointer>


#include <model/boostping/pingchecker.h>

//#include "../../model/boostping/pingchecker.h"
#include "view/Component/button.h"
#include "view/Component/customlabel.h"

class BoostPingView  : public QWidget
{
	Q_OBJECT

public:
	BoostPingView(QWidget *parent);
	~BoostPingView();

	CustomLabel* _status_label;

private:
	QLabel* _boostping_gif;
	//QPointer<PingChecker> _pingcheck;

	bool _first;
	bool _restartAfterBeforePing;
	bool _boostRunning;

	Button* _boost;
	Button* _unboost;

	CustomLabel* _unboostlabel;

	void checktimer();
signals:
	void boostPingOn();
	void startBoostPingTimer(int step);
	void startReSetBoostPingTimer();

	void getBeforePingSpeed();
	void boosPingNotify();
	void boosPingReSetNotify();

public Q_SLOTS:
	void updateStatusMsg(QString msg);
	
	
};
