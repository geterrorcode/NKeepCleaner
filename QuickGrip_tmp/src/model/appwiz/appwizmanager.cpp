#include "appwizmanager.h"

#include <windows.h>
#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QRegularExpression>

#include "view/common/alertpopup.h"
#include "../../common/tool/utils.h"
#include "../../controller/appwiz/appwizcontroller.h"


AppwizManager::AppwizManager()
{
	_uninstall_temp_data.clear();
	_uninstall_data.clear();
}

AppwizManager::~AppwizManager()
{
	if (_uninstall_data.size() > 0) {

		for (auto data : _uninstall_data)
		{
			if (data) {
				delete data;
			}
		}

		_uninstall_data.clear();
	}
	_uninstall_temp_data.clear();
}

void AppwizManager::updateAppWizData()
{
	
	_uninstall_data.clear();
	_uninstall_temp_data.clear();
	//getUninstallData();
	//getUninstallData(true);
	getUninstallData(HKEY_CURRENT_USER);
	getUninstallData(HKEY_LOCAL_MACHINE);
	getUninstallData(HKEY_LOCAL_MACHINE, true);

	//sort
	sortUninstallData();
	
}

//void AppwizManager::getUninstallData(bool HKCU, bool wow6432)
void AppwizManager::getUninstallData(HKEY rootkey, bool wow6432)
{
	HKEY hkey;

	DWORD enumindex = 0;
	TCHAR subkey[MAX_PATH];
	::ZeroMemory(subkey, MAX_PATH);
	DWORD buffersize = MAX_PATH;


	LONG result = 0;
	QString mainhKey = "HKEY_CURRENT_USER";
	if (rootkey == HKEY_LOCAL_MACHINE)
		mainhKey = "HKEY_LOCAL_MACHINE";

	if (wow6432) 
		result = RegOpenKeyEx(rootkey, REGKEY_UNINSTALL_WOW6432, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	else 
		result = RegOpenKeyEx(rootkey, REGKEY_UNINSTALL, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);

	if (result != ERROR_SUCCESS) 
		return;

	//int map_index = 0;
	while (::RegEnumKey(hkey, enumindex++, subkey, buffersize) != ERROR_NO_MORE_ITEMS) {

		QString reg_path = QString("%1\\%2\\").arg(mainhKey).arg(REGKEY_UNINSTALL);
		QString key_name = QString("%1").arg((LPWSTR)subkey);

		if (key_name == "N-KeepCleaner")
			continue;

		QSettings uninstallsetting(reg_path + key_name, QSettings::NativeFormat);
		QString name = uninstallsetting.value("DisplayName").toString();
		QString uninstallstr = uninstallsetting.value("UninstallString", "").toString();

		if (name.isEmpty() || uninstallstr.isEmpty()) //최소 이름,삭제 string 은 있어야 진행
			continue;

		QString iconPath = uninstallsetting.value("DisplayIcon", "").toString();
		
		auto data = new AppWizData();
		data->hmainkey = rootkey;
		data->keyname = key_name;
		data->program_name = name;
		data->uninstall_str = uninstallstr;
		data->iconpath = replaceQuotation(iconPath);
		data->publisher = uninstallsetting.value("Publisher", "_").toString();
		QString date = uninstallsetting.value("InstallDate").toString();
		if (date.isEmpty()) {
			if (!iconPath.isEmpty()) { //수정한 날짜
				QFileInfo FileInfo(iconPath);
				if (FileInfo.exists()) {
					date = FileInfo.lastModified().toString("yyyyMMdd");
				}
			}
		}
		data->install_date = date;

		data->version = uninstallsetting.value("DisplayVersion", "_").toString();

		size_t installsize = uninstallsetting.value("EstimatedSize", "0").toFloat();
		if (installsize == 0)
			data->size = "";
		else
			data->size = utils::getFileSizeType(installsize);
		
		_uninstall_temp_data.append(data);
	}


	RegCloseKey(hkey);
}

//쌍따움표 제거
QString AppwizManager::replaceQuotation(QString str)
{
	if (str.isEmpty())
		return "";

	return str.replace("\"", "");
}


void AppwizManager::sortUninstallData()
{
	std::sort(_uninstall_temp_data.begin(), _uninstall_temp_data.end(), [=](AppWizData* a, AppWizData* b) {

		if (a->install_date.isEmpty())
			return false;

		return a->install_date.toInt() > b->install_date.toInt();
		});

	for (auto data : _uninstall_temp_data) {
		_uninstall_data.insert(_uninstall_data.size(), data);
	}

}

void AppwizManager::delAppwizData(const int key)
{
	try
	{
		if (_uninstall_data.count(key) == 0) {
			AppwizController::inst()->appwizAlertCaller();
			//이미 삭제된 프로그램 입니다. 새로고침 눌러주세요
			return;
		}

		auto data = _uninstall_data[key];
		if (data) {

			QString keyname = data->keyname;
			if (keyname.isEmpty())
				return;

			//레지스트리 있냐 확인 (이미 삭제되었을수도 있어서 확인)
			HKEY hmainkey = data->hmainkey;
			QString mainkey;
			if (hmainkey == HKEY_CURRENT_USER)
				mainkey = "HKEY_CURRENT_USER";
			else
				mainkey = "HKEY_LOCAL_MACHINE";

			QString regpath = QString("%1\\%2\\%3").arg(mainkey).arg(REGKEY_UNINSTALL).arg(data->keyname);
			QSettings set(regpath, QSettings::NativeFormat);
			if (set.value("UninstallString").toString().isEmpty()) {
				AppwizController::inst()->appwizAlertCaller();
				//이미 삭제된 프로그램 입니다. 새로고침 눌러주세요
				return;
			}

			QString doubleq = "\"";
			QString uninstallcommand = replaceQuotation(data->uninstall_str);

			int index = uninstallcommand.indexOf(".exe") + 4;
			QString uninstaller_path = QString("%1%2%1").arg(doubleq, uninstallcommand.left(index));
			QString param = uninstallcommand.mid(index, -1);
			QStringList sl = param.split(QRegularExpression("[ ]"));

			QTimer::singleShot(300, [=]() {

				QProcess* process = new QProcess(this);
				process->start(uninstaller_path, sl);
				process->waitForStarted();

				QObject::connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
					[=](int exitCode, QProcess::ExitStatus exitStatus)
					{
						//여기서 한번더 체크
						QString regpath = QString("%1\\%2\\%3").arg(mainkey).arg(REGKEY_UNINSTALL).arg(data->keyname);
						QSettings set(regpath, QSettings::NativeFormat);
						if (set.value("UninstallString").toString().isEmpty()) {
							//삭제 완료
							_uninstall_data.remove(key);

							QObject::connect(this, SIGNAL(signalDelResult(int)), AppwizController::inst(), SLOT(slotdelResult(int)));
							emit this->signalDelResult(key);
						}
					});
				});
		}
	}
	catch (const std::exception&)
	{

	}
	
}

void AppwizManager::slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "finish";
}

