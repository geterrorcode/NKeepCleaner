#pragma once

#include <QObject>
#include <QMap>
#include <QThread>
#include <QPointer>

#include <Windows.h>


#include "optimizdata.h"
#include "deletethread.h"

#include "../../common/ux/singleton.h"

constexpr auto REGKEY_APPPATHS = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\";
constexpr auto REGKEY_APPPATHS_WOW6432 = L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\";
constexpr auto REGKEY_MODULEPATHS = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ModuleUsage\\";
constexpr auto REGKEY_MRU = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU";
constexpr auto REGKEY_WRONGSTARTUP = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
constexpr auto REGKEY_WRONGSTARTUP_WOW6432 = "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
constexpr auto REGKEY_WRONGUNINSTALL = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
constexpr auto REGKEY_WRONGUNINSTALL_WOW6432 = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
constexpr auto REGKEY_WRONGUNFIREWALL = "HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\FirewallRules";


class RegOptimizer  : public QObject,
	public common::ux::Singleton<RegOptimizer>
{
	Q_OBJECT

public:
	RegOptimizer(QObject *parent = Q_NULLPTR);
	~RegOptimizer();

public:

	void setRegCheck();
	LONGLONG getAllCheckRegCount();

	QString getRegTypeToString(RegOptimizeType type);

	QString getRegOptimizeCheckValue();
	QString getRegOptimizeValue();
	size_t getRegOptimizeFailValue();

	QMap<int, regOptimizeData>::iterator getRegScanFirstIter() {
		return _regOptimizeDatas.begin();
	}

	QMap<int, regOptimizeData>::iterator getRegScanLastIter() {
		return _regOptimizeDatas.end();
	}


	QList<regOptimizeData>::iterator getRegFailFirstIter() {
		return _regOptimizeFailDatas.begin();
	}

	QList<regOptimizeData>::iterator getRegFailLastIter() {
		return _regOptimizeFailDatas.end();
	}


private:
	LONGLONG _allcount;
	size_t _delcount;
	size_t _delfailcount;

	QPointer<DeleteThread> _deletethread;

	QMap<int, regOptimizeData> _regOptimizeDatas;
	QList<regOptimizeData> _regOptimizeFailDatas;
	QMap<RegOptimizeType, int> _regTypeCount;

	void insertRegOptimizeData(QString regpath, QString keyname, QString revalue, RegOptimizeType type);

	
	size_t getAppPathCount();
	size_t getActivexCount();
	size_t getMoudleCount();
	size_t getMRUCount();

	size_t getStartupCount();
	size_t getUninstallCount();
	size_t getFirewallRulesCount();

	void checkAppPaths(bool wow6432 = false);
	void checkWrongActiveX();
	void checkWrongModule();
	void checkWrongMRU(); 
	void checkWrongStartup(HKEY rootkey = HKEY_CURRENT_USER, bool wow6432 = false);
	void checkWrongUninstall(HKEY rootkey = HKEY_CURRENT_USER, bool wow6432 = false);
	void checkWrongFirewallRules();

	void progress();

public:
	void teminateDelThread();

signals:
	void regchecked(RegOptimizeType type);
	void regCheckComplete(int count);


	void sendRegoptimized(regOptimizeData data, bool delsuccess);
	void regOptimizeComplete(size_t allcount, size_t success_count);

public slots:
	void checkRegs();

	void optimizeRegs();
};
