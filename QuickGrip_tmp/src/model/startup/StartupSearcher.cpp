#include "StartupSearcher.h"

#include <stdlib.h>
#include <QtAlgorithms>
#include <vector>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFileIconProvider>
#include <shlobj_core.h>

#include "../../common/tool/utils.h"

void StartupSearcher::updateStartUp()
{
	_startup_info.clear();

	if (_startup_data.size() > 0)
		_startup_data.clear();

	updateStartup();

	//검색 완료 후 재정렬
	std::sort(_startup_info.begin(), _startup_info.end(), [=](const StartUpData& a, StartUpData& b) {
		if (a.filemake_date <= 0) {
			return false;
		}
		else
			return a.filemake_date > b.filemake_date;
		});

}


void StartupSearcher::updateStartup()
{
	

	//reg
	getRegValue(HKEY_CURRENT_USER, true);
	getRegValue(HKEY_CURRENT_USER, false);

	getRegValue(HKEY_LOCAL_MACHINE, true);
	getRegValue(HKEY_LOCAL_MACHINE, false);

	//start folder
	LPWSTR path;
	SHGetKnownFolderPath(FOLDERID_CommonStartup, 0, NULL, &path);
	QString startupPath = QString::fromWCharArray(path);
	getFolderValue(startupPath);

	SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &path);
	startupPath = QString::fromWCharArray(path);
	getFolderValue(startupPath);


}

void StartupSearcher::getRegValue(HKEY RoothKey, bool IsWow64)
{
	QString mainKey = "HKEY_LOCAL_MACHINE";
	if (RoothKey == HKEY_CURRENT_USER)
		mainKey = "HKEY_CURRENT_USER";

	QString keypath = "";
	if (IsWow64)
		keypath = QString("%1\\%2\\").arg(mainKey).arg(REGKEY_STARTUP);
	else
		keypath = QString("%1\\%2\\").arg(mainKey).arg(REGKEY_STARTUP_WOW6432);

	QSettings settings(keypath, QSettings::NativeFormat);
	QStringList regReturn = settings.allKeys();

	for (auto key : regReturn)
	{
		if (key == "N-KeepCleaner")
			continue;

		QString value = settings.value(key).toString();
		if (value.isEmpty())
			continue;

		setStartupInfo(key, value, StartUpType::regType, keypath + key);
	}

	//settings.clear();

}

void StartupSearcher::enumSubRegKeys(HKEY RoothKey, bool IsWow64, std::vector<QString>& keyNames)
{
	keyNames.clear();

	LONG lResult = ERROR_SUCCESS;
	HKEY hKey;

	if (IsWow64 == true)
		lResult = RegOpenKeyEx(RoothKey, REGKEY_STARTUP, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	else
		lResult = RegOpenKeyEx(RoothKey, REGKEY_STARTUP, 0, KEY_ALL_ACCESS, &hKey);

	if (lResult != ERROR_SUCCESS) return;


	DWORD dwMaxValueLen = 0;
	DWORD lpcValues = 0;

	lResult = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &lpcValues, NULL, &dwMaxValueLen, NULL, NULL);
	if (lResult != ERROR_SUCCESS) return;

	DWORD i = 0;
	TCHAR lpValue[8000] = { 0, };
	DWORD Size = 8000;
	DWORD dwType = REG_NONE;
	LPBYTE lpData = new BYTE[dwMaxValueLen];
	DWORD cbData = dwMaxValueLen;

	for (i = 0; lResult == ERROR_SUCCESS; i++) 	{
		ZeroMemory(lpData, dwMaxValueLen);
		ZeroMemory(lpValue, sizeof(lpValue));
		Size = 8000;
		cbData = dwMaxValueLen;

		lResult = RegEnumValue(hKey, i, lpValue, &Size, NULL, &dwType, lpData, &cbData);
		if (lResult == ERROR_SUCCESS) {
			if (Size > 0 && cbData > 0) {
				keyNames.push_back(QString::fromWCharArray(lpValue));
			}
		}
	}

	delete[] lpData;
}

void StartupSearcher::getSubRegKeyValue(HKEY RoothKey, bool IsWow64, QString keyName)
{
	QString mainKey = "HKEY_LOCAL_MACHINE";
	if (RoothKey == HKEY_CURRENT_USER)
		mainKey = "HKEY_CURRENT_USER";

	QString keypath = "";
	if (IsWow64)
		keypath = QString("%1\\%2").arg(mainKey).arg(REGKEY_STARTUP);
	//keypath = QString("%1\\%2\\%3").arg(mainKey).arg(REGKEY_STARTUP).arg(keyName);
	else
		keypath = QString("%1\\%2").arg(mainKey).arg(REGKEY_STARTUP_WOW6432);
		
	QSettings settings(keypath, QSettings::NativeFormat);
	QStringList regReturn = settings.allKeys();

	for (auto key : regReturn)
	{
		QString value = settings.value(key).toString();
		if (value.isEmpty())
			continue;

		setStartupInfo(key, value, StartUpType::regType, keypath.append("\\").append(key));

		/*StartUpData data;
		data.namne = key;
		data.file_path = value;
		data.type = StartUpType::regType;
		data.regPath = keypath.append("\\").append(key);

		_startup_data.insert(_startup_data.count(), data);*/
	}

	//settings.clear();
}

void StartupSearcher::getFolderValue(QString folderPath)
{
	QDir startup_folder(folderPath);
	if(!startup_folder.exists())
		return;

	QStringList files = startup_folder.entryList();
	for (auto file : files)
	{
		if (file == "." || file == "..")
			continue;

		QString fullPath = startup_folder.absolutePath().append("/").append(file);

		setStartupInfo(file, fullPath, StartUpType::fileType, fullPath);

		/*StartUpData data;
		data.namne = file;
		data.file_path = fullPath;
		data.type = StartUpType::fileType;
		data.regPath = fullPath;

		_startup_data.insert(_startup_data.count(), data);*/
	}

}


void StartupSearcher::setStartupInfo(QString namne, QString filepath, StartUpType type, QString regPath)
{
	QString path;
	if (type == StartUpType::regType) {
		QString temp = filepath.replace("\"", "");
		int index = temp.indexOf(".exe") + 4;
		path = temp.left(index);
	}
	else {
		path = filepath;
	}



	QString date = utils::getFileMakeDate(path);
	StartUpData data;
	data.namne = namne;
	data.file_path = filepath;
	data.type = type;
	data.regPath = regPath;
	data.filemake_date = date.toInt();

	//qDebug() << "Naem : " << namne << ", Date : " << date << ", Path : " << filepath;

	_startup_info.append(data);
	//_startup_data.insert(_startup_data.count(), data);
}


void StartupSearcher::deleteStartUpList(QList<int> items)
{
	try
	{
		for (auto item : items)
		{
			auto info = _startup_info.at(item);
			if (info.type == StartUpType::fileType) {

				if (QFile::exists(info.file_path))
					QFile::remove(info.file_path);
			}
			else { //reg

				int index = info.regPath.lastIndexOf("\\");
				if (index < 1)
					return;

				QString regpath = info.regPath.left(index );
				QString regvalue = info.regPath.mid(index + 1, info.regPath.length());

				QSettings settings(regpath, QSettings::NativeFormat);
				settings.remove(regvalue);

			}

			_startup_info.removeAt(item);
		}
	}
	catch (const std::exception&)
	{
		return;
	}
	
	//_startup_info.rem
}

void StartupSearcher::slotgetStartupData()
{
	updateStartUp();

}

void StartupSearcher::slotsearStartUp()
{

}