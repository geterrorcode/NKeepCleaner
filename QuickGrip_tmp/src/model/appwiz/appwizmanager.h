#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include <QProcess>
#include <Windows.h>

#include "appwizdata.h"

#include "../../common/ux/singleton.h"

constexpr auto REGKEY_UNINSTALL = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
constexpr auto REGKEY_UNINSTALL_WOW6432 = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

class AppwizManager : QObject, 
	public common::ux::Singleton<AppwizManager>
{

	Q_OBJECT

	//QVariantMap map;

public:
	AppWizData* gatApp(const int& index);
	AppwizManager();
	~AppwizManager();

public:
	void updateAppWizData();

	int getDataSize() const {
		return _uninstall_data.size();
	}

	QMap<int, AppWizData*>::iterator getFirstIter() {
		return _uninstall_data.begin();
	}

	QMap<int, AppWizData*>::iterator getLastIter() {
		return _uninstall_data.end();
	}


	void delAppwizData(const int key);
	

private:
	//QVector<AppWizData*> _a;
	QVector<AppWizData*> _uninstall_temp_data;
	QMap<int, AppWizData*> _uninstall_data;

private:
	//void getUninstallData(bool HKCU = false, bool wow6432 = false);
	void getUninstallData(HKEY rootkey, bool wow6432 = false);
	QString replaceQuotation(QString str);
	void sortUninstallData();
	

signals:
	void signalDelResult(int);

public Q_SLOT:
	void slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

};

//inline const AppwizManager* appwizManager() { return &AppwizManager::cinst(); }