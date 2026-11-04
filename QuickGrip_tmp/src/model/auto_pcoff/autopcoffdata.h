#pragma once

#include <QObject>
#include <QFile>
#include <QPointer>
#include <QTimer>
#include <QMutex>

#include <model/smart_optimize/smartopimizedata.h>

#include <common/defines/defineproperty.h>
#include <common/ux/singleton.h>

enum class AutoPCOffStatus
{
	NONE = 0,
	NOTUSE,
	ALERT,
	PCOFF
};


class AutoPcOffData  : public QObject,
	public common::ux::Singleton<AutoPcOffData>
{
	Q_OBJECT

public:
	AutoPcOffData(QObject *parent = Q_NULLPTR);
	~AutoPcOffData();

private:
	QMutex _mutex;
	bool _alertflag;
	bool _pcoffflag;


	ReserveData _autoPcffInfo;
	QString _jspnpath;
	std::shared_ptr<QTimer> _checkTimer;
	void timerCallbackFunc();
	void timerStart();
	void timerStop();
	AutoPCOffStatus autoPcoffCheck();
	

	bool updateData();
	void setJsonData();
	void parseJsonData(const QJsonObject& json);
public:
	bool isAutoPcOffUse() const;
	ReserveData getPcoffData() const;
	void setPcOffData(ReserveData data);
	void setPcOffOnOff(bool off);

Q_SIGNALS:
	void callPcOffAlert();
	void callPCOff();
};
