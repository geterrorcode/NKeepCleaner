#include "regoptimizer.h"

#include <windows.h>
#include <QSettings>
#include <QFile>

#include <QTimer>
#include <QThread>

QMap<RegOptimizeType, QString> _regTypetoName = {
	{RegOptimizeType::RegWrongAppPath, "間違ったプログラムの実行パス"},//잘못된 프로그램 실행 경로
	{RegOptimizeType::RegWrongActiveX, "間違ったActiveX パス"},//잘못된 ActiveX 경로
	{RegOptimizeType::RegWrongModuleUsage, "間違ったモジュールパス"},//잘못된 모듈 경로
	{RegOptimizeType::RegWrongMRU, "間違ったMRUパス"},//잘못된 MRU 경로
	{RegOptimizeType::RegWrongStartup, "間違ったスタートプログラムパス"},//잘못된 시작프로그램 경로
	{RegOptimizeType::RegWrongUninstall, "間違ったプログラム設置パス"},//잘못된 프로그램 설치 경로
	{RegOptimizeType::RegWrongFirewallRules, "間違ったファイアウォールポリシー"},//잘못된 방화벽 정책
	{RegOptimizeType::none, "未分類されたレジストリエラー"} //미 분류된 레지스트리 오류
};






RegOptimizer::RegOptimizer(QObject *parent)
	: QObject(parent), _allcount(0), _delcount(0), _delfailcount(0), _deletethread(Q_NULLPTR)
{
	_regOptimizeDatas.clear();
	_regOptimizeFailDatas.clear();
	_regTypeCount.clear();
}

RegOptimizer::~RegOptimizer()
{
	if (!_deletethread.isNull()) {
		_deletethread->deleteLater();
		_deletethread = Q_NULLPTR;
	}
}


QString RegOptimizer::getRegTypeToString(RegOptimizeType type)
{
	if (_regTypetoName.contains(type)) {
		return _regTypetoName[type];
	}

	return "未分類されたレジストリエラー";
}

QString RegOptimizer::getRegOptimizeCheckValue()
{
	return QString("%1").arg(_regOptimizeDatas.size());
}

QString RegOptimizer::getRegOptimizeValue()
{
	return QString("%1").arg(_delcount);
}

size_t RegOptimizer::getRegOptimizeFailValue()
{
	return _delfailcount;
}

void RegOptimizer::setRegCheck()
{
	_regOptimizeDatas.clear();
	_regOptimizeFailDatas.clear();

	//_checkcount = 0;

		checkAppPaths();
		//checkAppPaths(true);
		checkWrongActiveX();
		checkWrongModule();
		checkWrongMRU();

		checkWrongStartup(HKEY_CURRENT_USER);
		/*checkWrongStartup(HKEY_LOCAL_MACHINE);
		checkWrongStartup(HKEY_LOCAL_MACHINE, true);*/

		checkWrongUninstall(HKEY_CURRENT_USER);
		//checkWrongUninstall(HKEY_LOCAL_MACHINE);
		//checkWrongUninstall(HKEY_LOCAL_MACHINE, true);

		checkWrongFirewallRules();
		//});
}

void RegOptimizer::insertRegOptimizeData(QString regpath, QString keyname, QString revalue, RegOptimizeType type)
{

	//qDebug() << "RegPath : " << regpath << " ,KeyName : " << keyname << " ,regValue : " << revalue << " ,Type : " << (int)type;

	regOptimizeData data;
	data.regpath = regpath;
	data.keyname = keyname;
	data.regvalue = revalue;
	data.optimizetype = type;
	_regOptimizeDatas.insert(_regOptimizeDatas.size(), data);
}


LONGLONG RegOptimizer::getAllCheckRegCount()
{
	LONGLONG allcount = 0;

	allcount += getAppPathCount();
	//allcount += getActivexCount();
	allcount += getMoudleCount();
	allcount += getMRUCount();
	allcount += getStartupCount();
	allcount += getUninstallCount();
	//allcount += getFirewallRulesCount();

	return allcount;

}

size_t RegOptimizer::getAppPathCount()
{
	//HKEY_LOCAL_MACHINE, REGKEY_APPPATHS
	size_t count = 0;
	//HKCU만 검사
	QString regpath = QString("HKEY_CURRENT_USER\\%1").arg(REGKEY_APPPATHS);
	QSettings reg(regpath, QSettings::NativeFormat);
	count = reg.childGroups().count();

	//관리자 권한 해제로 HKLM 제거
	/*QString regpath = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_APPPATHS);
	QSettings reg(regpath, QSettings::NativeFormat);
	count = reg.childGroups().count();

	QString regpath_wow6432 = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_APPPATHS_WOW6432);
	QSettings reg_wow6432(regpath_wow6432, QSettings::NativeFormat);
	count += reg_wow6432.childGroups().count();*/

	return count;
	
}

size_t RegOptimizer::getActivexCount()
{
	//HKEY_CLASSES_ROOT, L"CLSID"
	QString regpath = QString("HKEY_CLASSES_ROOT\\CLSID");
	QSettings reg(regpath, QSettings::NativeFormat);
	return reg.childGroups().count();
}

size_t RegOptimizer::getMoudleCount()
{

	//HKEY_LOCAL_MACHINE, REGKEY_MODULEPATHS
	//QString regpath = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_MODULEPATHS);
	//관리자 권한 제거로 로컬머신 제거
	QString regpath = QString("HKEY_CURRENT_USER\\%1").arg(REGKEY_MODULEPATHS);
	QSettings reg(regpath, QSettings::NativeFormat);
	return reg.childGroups().count();
}

size_t RegOptimizer::getMRUCount()
{
	QSettings reg(REGKEY_MRU, QSettings::NativeFormat);
	return reg.childKeys().count();
}

size_t RegOptimizer::getStartupCount()
{
	size_t count = 0;

	QString regpath = QString("HKEY_CURRENT_USER\\%1").arg(REGKEY_WRONGSTARTUP);
	QSettings reg_hkcu(regpath, QSettings::NativeFormat);
	count = reg_hkcu.allKeys().count();

	/*regpath = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_WRONGSTARTUP);
	QSettings reg_hklm(regpath, QSettings::NativeFormat);
	count += reg_hklm.allKeys().count();

	regpath = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_WRONGSTARTUP_WOW6432);
	QSettings reg_hklm_wow6432(regpath, QSettings::NativeFormat);
	count += reg_hklm_wow6432.allKeys().count();*/

	return count;
}

size_t RegOptimizer::getUninstallCount()
{
	//REGKEY_WRONGUNINSTALL_WOW6432/
	size_t count = 0;

	QString regpath = QString("HKEY_CURRENT_USER\\%1").arg(REGKEY_WRONGUNINSTALL);
	QSettings reg_hkcu(regpath, QSettings::NativeFormat);
	count = reg_hkcu.childGroups().count();

	/*regpath = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_WRONGUNINSTALL);
	QSettings reg_hklm(regpath, QSettings::NativeFormat);
	count += reg_hklm.childGroups().count();

	regpath = QString("HKEY_LOCAL_MACHINE\\%1").arg(REGKEY_WRONGUNINSTALL_WOW6432);
	QSettings reg_hklm_wow6432(regpath, QSettings::NativeFormat);
	count += reg_hklm_wow6432.childGroups().count();*/

	return count;
}

size_t RegOptimizer::getFirewallRulesCount()
{
	QSettings reg(REGKEY_WRONGUNFIREWALL, QSettings::NativeFormat);
	return reg.childKeys().count();
}

//Search_Reg_Wrong_AppPath
//アプリケーション・プログラム (%d 個)
void RegOptimizer::checkAppPaths(bool wow6432)
{
	HKEY hkey;
	DWORD enumindex = 0;
	TCHAR subkey[MAX_PATH];
	::ZeroMemory(subkey, MAX_PATH);
	DWORD buffersize = MAX_PATH;
	LONG result = 0;

	QString path;
	if (wow6432) {
		path = QString("%1").arg(REGKEY_APPPATHS_WOW6432);
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_APPPATHS_WOW6432, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	}
	else {
		path = QString("%1").arg(REGKEY_APPPATHS);
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_APPPATHS, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	}

	if (result != ERROR_SUCCESS)
		return;

	while (::RegEnumKey(hkey, enumindex++, subkey, buffersize) != ERROR_NO_MORE_ITEMS) {

		emit regchecked(RegOptimizeType::RegWrongAppPath);

		//QString reg_path = QString("HKEY_LOCAL_MACHINE\\%2\\").arg(path);
		//wprj
		QString reg_path = QString("HKEY_CURRENT_USER\\%2\\").arg(path);
		QString key_name = QString("%1").arg((LPWSTR)subkey);

		QSettings apppath_reg(reg_path + key_name, QSettings::NativeFormat);
		QString default_value = apppath_reg.value("Default", "0").toString();
		if (default_value == "0")
			continue;

		if (!QFile::exists(default_value.replace("\"", "")) && default_value.indexOf("%") < 0) {
			insertRegOptimizeData(reg_path, key_name, default_value, RegOptimizeType::RegWrongAppPath);
		}

	}

	RegCloseKey(hkey);
}


//ユーザー・コントロール (%d 個)
void RegOptimizer::checkWrongActiveX()
{
	HKEY hkey;
	DWORD enumindex = 0;
	TCHAR subkey[MAX_PATH];
	::ZeroMemory(subkey, MAX_PATH);
	DWORD buffersize = MAX_PATH;

	LONG result = RegOpenKeyEx(HKEY_CLASSES_ROOT, L"CLSID", 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	if (result != ERROR_SUCCESS)
		return;

	while (::RegEnumKey(hkey, enumindex++, subkey, buffersize) != ERROR_NO_MORE_ITEMS) {

		emit regchecked(RegOptimizeType::RegWrongActiveX);
		
		QString key_name = QString("%1").arg((LPWSTR)subkey);
		QString clsidpath = QString("HKEY_CLASSES_ROOT\\CLSID\\%1\\InprocServer32").arg(key_name);
		QSettings clsid_reg(clsidpath, QSettings::NativeFormat);

		QString default_value = clsid_reg.value("Default", "0").toString();
		if (default_value == "0")
			continue;
		
		default_value.replace("\x22", "");
		if (!QFile::exists(default_value) && 
			default_value.indexOf("%") < 0 && 
			default_value.indexOf(".dll") < 0 &&
			default_value.indexOf("\\") > -1 ) {

			insertRegOptimizeData(clsidpath, "Default", default_value, RegOptimizeType::RegWrongActiveX);
		}

	}

	RegCloseKey(hkey);
}

//モジュール(% d 個)
void RegOptimizer::checkWrongModule()
{
	HKEY hkey;
	DWORD enumindex = 0;
	TCHAR subkey[MAX_PATH];
	::ZeroMemory(subkey, MAX_PATH);
	DWORD buffersize = MAX_PATH;

	//LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_MODULEPATHS, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	//제거
	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY_MODULEPATHS, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	if (result != ERROR_SUCCESS)
		return;

	while (::RegEnumKey(hkey, enumindex++, subkey, buffersize) != ERROR_NO_MORE_ITEMS) {

		emit regchecked(RegOptimizeType::RegWrongModuleUsage);
		
		QString key_name = QString("%1").arg((LPWSTR)subkey);
		key_name.replace("\x22", "");
		if (!QFile::exists(key_name) &&
			key_name.indexOf("%") < 0 ) {

			//insertRegOptimizeData(QString("HKEY_LOCAL_MACHINE\\%1%2").arg(REGKEY_MODULEPATHS), key_name, key_name, RegOptimizeType::RegWrongModuleUsage);
			//제거
			insertRegOptimizeData(QString("HKEY_CURRENT_USER\\%1%2").arg(REGKEY_MODULEPATHS), key_name, key_name, RegOptimizeType::RegWrongModuleUsage);
		}
	}

	RegCloseKey(hkey);
}

void RegOptimizer::checkWrongMRU()
{
	QSettings mrumain(REGKEY_MRU, QSettings::NativeFormat);
	QStringList list = mrumain.childKeys();

	QString mrulist = mrumain.value("MRUList", "").toString();
	if (mrulist.isEmpty())
		return;

	int count = 0;
	for (size_t i = 0; i < mrulist.size(); i++)
	{
		emit regchecked(RegOptimizeType::RegWrongMRU);
		QString valuename = mrulist[i];

		if (valuename == Q_NULLPTR)
			continue;

		if (valuename.isEmpty())
			continue;

		QString mru = mrumain.value(valuename, "").toString().replace("\\1", "");
		if (mru.indexOf("\\") > -1 && !QFile::exists(mru)) {

			insertRegOptimizeData(QString("%1").arg(REGKEY_MRU), valuename, mru, RegOptimizeType::RegWrongMRU);
			count++;
		}
	}
}

void RegOptimizer::checkWrongStartup(HKEY rootkey, bool wow6432)
{
	QString mainKey = "HKEY_CURRENT_USER";
	QString startupreg = QString("%1").arg(REGKEY_WRONGSTARTUP);
	if (rootkey == HKEY_LOCAL_MACHINE) {
		mainKey = "HKEY_LOCAL_MACHINE";
		if (wow6432) {
			startupreg = QString("%1").arg(REGKEY_WRONGSTARTUP_WOW6432);
		}
	}

	QString keypath = QString("%1\\%2").arg(mainKey).arg(startupreg);

	QSettings settings(keypath, QSettings::NativeFormat);
	QStringList regReturn = settings.allKeys();
	int size = regReturn.size();

	for (auto key : regReturn)
	{
		emit regchecked(RegOptimizeType::RegWrongStartup);

		QString value = settings.value(key).toString();
		value.replace("\x22", "");

		if (value.isEmpty())
			continue;

		if (value.indexOf(".exe") < 0 || value.indexOf("%") > -1)
			continue;

		int index = value.indexOf(".exe") + 4;
		QString filepath = value.left(index);
		if (!QFile::exists(filepath)) {

			insertRegOptimizeData(keypath, key, value, RegOptimizeType::RegWrongStartup);
		}

	}
}

void RegOptimizer::checkWrongUninstall(HKEY rootkey, bool wow6432)
{
	if (rootkey == 0)
		return;

	HKEY hkey;

	DWORD enumindex = 0;
	TCHAR subkey[MAX_PATH];
	::ZeroMemory(subkey, MAX_PATH);
	DWORD buffersize = MAX_PATH;

	LONG result = 0;
	
	QString mainKey = "HKEY_CURRENT_USER";
	if (rootkey == HKEY_LOCAL_MACHINE)
		mainKey = "HKEY_LOCAL_MACHINE";

	QString wow6432reg;

	if (wow6432) {
		wow6432reg = QString("%1").arg(REGKEY_WRONGUNINSTALL_WOW6432);
		result = RegOpenKeyEx(rootkey, REGKEY_WRONGUNINSTALL_WOW6432, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	}
	else {
		wow6432reg = QString("%1").arg(REGKEY_WRONGUNINSTALL);
		result = RegOpenKeyEx(rootkey, REGKEY_WRONGUNINSTALL, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	}
	
	if (result != ERROR_SUCCESS)
		return;

	//int map_index = 0;
	while (::RegEnumKey(hkey, enumindex++, subkey, buffersize) != ERROR_NO_MORE_ITEMS) {

		emit regchecked(RegOptimizeType::RegWrongUninstall);

		QString reg_path = QString("%1\\%2\\").arg(mainKey).arg(wow6432reg);
		QString key_name = QString("%1").arg((LPWSTR)subkey);

		QSettings uninstallsetting(reg_path + key_name, QSettings::NativeFormat);
		QString name = uninstallsetting.value("DisplayName").toString();
		if (name.isEmpty()) {
			insertRegOptimizeData(reg_path , key_name, "空のレジストリ", RegOptimizeType::RegWrongUninstall);
			continue;
		}


		QString uninstallstr = uninstallsetting.value("UninstallString", "").toString();
		if (uninstallstr.isEmpty()) {
			insertRegOptimizeData(reg_path , key_name, name, RegOptimizeType::RegWrongUninstall);
			continue;
		}
		
		uninstallstr.replace("\x22", "");
		if (uninstallstr.indexOf(".exe") < 0 || uninstallstr.indexOf("%") > -1)
			continue;

		int index = uninstallstr.indexOf(".exe") + 4;
		QString filepath = uninstallstr.left(index);
		if (filepath.indexOf("\\") > -1 && !QFile::exists(filepath)) {

			insertRegOptimizeData(reg_path, key_name, name, RegOptimizeType::RegWrongUninstall);
		}
	}

	RegCloseKey(hkey);
}

void RegOptimizer::checkWrongFirewallRules()
{

	QSettings apppath_reg(REGKEY_WRONGUNFIREWALL, QSettings::NativeFormat);
	QStringList rules = apppath_reg.childKeys();
	for (auto rule : rules)
	{
		emit regchecked(RegOptimizeType::RegWrongFirewallRules);


		//progress();
		QString value = apppath_reg.value(rule).toString();
		if (value.isEmpty())
			continue;

		int firstindex = value.indexOf("App=");
		int lastindex = value.indexOf(".exe");

		if (firstindex < 0 || lastindex < 0)
			continue;

		firstindex += 4;
		lastindex += 4;
		int midlast = lastindex - firstindex;
		if (midlast < 0)
			continue;

		QString filepath = value.mid(firstindex, midlast);
		filepath.replace("\x22", "");
		if (filepath.indexOf("%") < 0 && filepath.indexOf("\\") > -1 && !QFile::exists(filepath)) {

			insertRegOptimizeData(QString("%1").arg(REGKEY_WRONGUNFIREWALL), rule, filepath, RegOptimizeType::RegWrongFirewallRules);
		}

	}

	//_checkcount = _allcount;
	//progress();
}

void RegOptimizer::progress()
{
	//int progress = (float)_checkcount / _allcount * 100;
}

void RegOptimizer::teminateDelThread()
{
	if (_deletethread.isNull() == false) {

		
		if (_deletethread->isRunning()) {
			_deletethread->wait(3000);
			_deletethread->quit();
			/*_deletethread->wait(3000);
			_deletethread->terminate();*/
		}
		//;delete _deletethread;
		//_deletethread = Q_NULLPTR;
	}
}

void checkthread() {
	forever{

		RegOptimizer::inst()->setRegCheck();
		break;

		if (QThread::currentThread()->isInterruptionRequested()) {
			return;
		}
	}
}

void RegOptimizer::checkRegs()
{
	auto thread = QThread::create(checkthread); 
	thread->start();
	connect(thread, &QThread::finished, this, [=]() {

		emit regCheckComplete(_regOptimizeDatas.size());
		
		});

	//_regOptimizeDatas
	//setRegCheck();
}

void RegOptimizer::optimizeRegs()
{
	_delcount = 0;

	if (_deletethread.isNull() == false) {
		delete _deletethread;
		_deletethread = Q_NULLPTR;
	}

	_deletethread = new DeleteThread(this);
	_deletethread->setDelRegInfo(_regOptimizeDatas);

	//파일 한개 완료
	QObject::connect(_deletethread, &DeleteThread::snedDeleteReg, [=](regOptimizeData data, bool delsuccess) {

		if (delsuccess) 
			_delcount++;
		else {
			qDebug() << "Delete Error! : " << data.regpath;
			_delfailcount++;

			_regOptimizeFailDatas.push_back(data);
		}
			

		emit sendRegoptimized(data, delsuccess);
		});


	QObject::connect(_deletethread, &DeleteThread::snedDeleteRegComplete, [=]() {
		//_deletethread->terminate();
		//_deletethread->deleteLater();

		emit regOptimizeComplete(_regOptimizeDatas.count(), _delcount);
		});


	_deletethread->start();
}