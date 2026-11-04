#pragma once

#include <wtypes.h>
#include <QString>
#include <QMap>
#include <QVector>

#include "startupdata.h"

#include "../../common/ux/singleton.h"

constexpr auto REGKEY_STARTUP = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
constexpr auto REGKEY_STARTUP_WOW6432 = L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";

//bool operator<(const StartUpData& a, const StartUpData& b) { return a.filemake_date < b.filemake_date; }

class StartupSearcher : public common::ux::Singleton<StartupSearcher>
{
private:
	QVector<StartUpData> _startup_info;
	QMap<int, StartUpData> _startup_data;
	
	

	void updateStartup();
	void getRegValue(HKEY RoothKey, bool IsWow64);
	void enumSubRegKeys(HKEY RoothKey, bool IsWow64, std::vector<QString>& keyNames);
	void getSubRegKeyValue(HKEY RoothKey, bool IsWow64, QString keyName);
	void getFolderValue(QString folderPath);

	void setStartupInfo(QString namne, QString filepath, StartUpType type, QString regPath);

	//QIcon getFileIcon(QDir path, QString fileName);


public:
	void updateStartUp();

	int getDataSize() const {
		return _startup_data.size();
	}


	QVector<StartUpData>::iterator getFirstIter() {
		return _startup_info.begin();
		//return _startup_data.begin();
	}

	QVector<StartUpData>::iterator getLastIter() {
		return _startup_info.end();
		//return _startup_data.end();
	}

	
	//QMap<int, StartUpData>::iterator getFirstIter() {
	//	return __startup_info.begin();
	//	//return _startup_data.begin();
	//}

	//QMap<int, StartUpData>::iterator getLastIter() {
	//	return __startup_info.begin();
	//	//return _startup_data.end();
	//}


	//void delStartUp(const int key);
	void deleteStartUpList(QList<int> items);

public  slots:
	void slotgetStartupData();
	void slotsearStartUp();

};

//inline const StartupSearcher* startupSearcher() { return &StartupSearcher::cinst(); }