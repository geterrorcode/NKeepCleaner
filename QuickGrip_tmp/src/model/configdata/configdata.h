#pragma once

#include <QObject>
#include <QWidget>
#include <QFile>

#include "../../common/defines/defineproperty.h"
#include "../../common/ux/singleton.h"

//struct ConfigData
//{
//	bool tray;
//	bool startopt;
//
//	ConfigData() {
//		tray = false;
//		startopt = false;
//	}
//};

class ConfigData  : public QObject,
	public common::ux::Singleton<ConfigData>
{
	Q_OBJECT

	//PROPERTY(int, tryopt, setTryopt)

public:
	ConfigData(QObject *parent = Q_NULLPTR);
	~ConfigData();


private:
	QString _configfile_path;
	//QFile _config_file;

	QString _serial_key;
	QString _uuid;

	bool _tray_opt;
	bool _startup_opt;
	//bool _regoptimize_opt;
	//bool _start_program_opt;
	//bool _uninstall_opt;

	QString _date;
	QString _time;
	int _diff_days;

	bool _auto_renewal;
	bool _subscribing;
	int _expired_days;
	QString _expired_date;

	QString _contractId;
	QString _nextBillingDate;
	QString _billings;

	void initConfig();
	void setConfigValue(QString key, bool opt);

	void setAuthInfo();

	void updateScanDiffDay();

	void setTaskSch(bool use);

public:
	void setStartupOpt(bool opt);
	void setStartupOptOnly(bool opt);

	void setAuthInfo(QString serial, QString uuid);
	void clearAuthinfo();

	void setSerialKey(QString serial);
	QString getSerialKey() const { return _serial_key; }

	void setUUID(QString uuid);
	QString getUUID() const { return _uuid; }
	QString getOldFileUUID();

	//void setRegOptimizeOpt(bool opt);
	//bool getRegOptimizeOpt() const;

	//void setStartProgramOpt(bool opt);
	//bool getStartProgramOpt() const;

	//void setUninstallOpt(bool opt);
	//bool getUninstallOpt() const;

	bool getStartupOpt() const;
	void setTrayOpt(bool opt);

	bool getTrayOpt() const;
	int getDiffDays() const;


	QString getScanDate() const	{return _date;}
	QString getScanTime() const { return _time; }

	bool isAutoRenewal() const { return _auto_renewal; }
	void setAutoRenewal(bool renewal) { _auto_renewal = renewal; }

	bool isSubscribe() const { return _subscribing; }
	void setSubscribe(bool sub) { _subscribing = sub; }

	void setExpiredDays(int days) { _expired_days = days; }
	int getExpiredDays() const { return _expired_days; }

	void setExpiredDate(QString date) { _expired_date = date; }
	QString getExpiredDate() const { return _expired_date; }

	void setContractId(QString cid) { _contractId = cid; }
	QString getContractId() const { return _contractId; }

	void setNextBillingDate(QString date) { _nextBillingDate = date; }
	QString getNextBillingDate() const { return _nextBillingDate; }

	void setBillings(QString billings) { _billings = billings; }
	QString getBillings() const { return _billings; }


Q_SIGNALS:
	void setScanDiffDay();
	void setLockStartupOpt();
	void setChangeStartupOpt();

public Q_SLOTS:
	void updateOptimizeDate();


};
