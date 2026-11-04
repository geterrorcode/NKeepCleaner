#include "configdata.h"

#include <QStandardPaths>
#include <QTextStream>
#include <QDir>
#include <QjsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDateTime>


#include <view/option/optionview.h>
#include <model/smart_optimize/smartoptimizeinfo.h>
#include <model/auto_pcoff/autopcoffdata.h>
#include <common/tool/utils.h>

#include "../../ui.h"



ConfigData::ConfigData(QObject* parent)
	: QObject(parent), _tray_opt(false), _startup_opt(true),
	_configfile_path(""), _serial_key(""), _uuid(""),
	_date("0"), _time("0"), _diff_days(-1), _subscribing(false), _expired_days(-1) , _expired_date("")
{

//#ifdef _DEBUG
//	_subscribing = true;
//#endif


	QString configpath = QString("%1/cfg/").arg(qApp->applicationDirPath());
	if (QDir(configpath).exists() == false)
		QDir().mkdir(configpath);

	_configfile_path = QString("%1/config.json").arg(configpath);

	initConfig();

	setAuthInfo();

#ifdef DEBUG
	setUUID(utils::getSystemUUID());
	//setUUID("anyopen");
#else
	setUUID(utils::getSystemUUID());
#endif // DEBUG

	//setUUID(utils::getSystemUUID());
	//anyopen

	/*if (SmartOptimizeInfo::inst()->getUseData() == true || AutoPcOffData::inst()->isAutoPcOffUse() == true) {
		if (_startup_opt == false) {
			setStartupOptOnly(true);
		}
	}*/

	//setTaskSch(_startup_opt);
	setTaskSch(false);

}

ConfigData::~ConfigData()
{
	//if (_config_file.isOpen())
	//	_config_file.close();
}


void ConfigData::initConfig()
{
	QFile config_file(_configfile_path);
	if (config_file.open(QIODevice::ReadOnly)) {
		QByteArray bytes = config_file.readAll();

		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);

		if (jsonError.error != QJsonParseError::NoError)
			return;

		if (document.isObject()) {

			QJsonObject jsonObj = document.object();
			QJsonObject obj = jsonObj.value("config").toObject();
			auto trayopt = obj.take("tray");
			if (trayopt.isBool())
				_tray_opt = trayopt.toBool();

			//무조건 false
			//auto startup = obj.take("startup");
			//if (startup.isBool())
			//	_startup_opt = startup.toBool();
			_startup_opt = false;

			//auto regoptimize = obj.take("regoptimize");
			//if (regoptimize.isBool())
			//	_regoptimize_opt = regoptimize.toBool();

			//auto start_program = obj.take("start_program");
			//if (start_program.isBool())
			//	_start_program_opt = start_program.toBool();

			//auto uninstall = obj.take("uninstall");
			//if (uninstall.isBool())
			//	_uninstall_opt = uninstall.toBool();

			QJsonObject obj_scan_date = jsonObj.value("scandate").toObject();
			auto date = obj_scan_date.take("date");
			if (date.isString())
				_date = date.toString();

			auto time = obj_scan_date.take("time");
			if (time.isString())
				_time = time.toString();

			updateScanDiffDay();
		}

	}
	else { //default set
		auto config = QJsonObject(
			{
			qMakePair(QString("startup"), QJsonValue(false)),
			qMakePair(QString("tray"), QJsonValue(false))
			//qMakePair(QString("regoptimize"), QJsonValue(false)),
			//qMakePair(QString("startprogram"), QJsonValue(false)),
			//qMakePair(QString("uninstall"), QJsonValue(false))
			});

		auto scandate = QJsonObject(
			{
			qMakePair(QString("date"), QJsonValue("0")),
			qMakePair(QString("time"), QJsonValue("0"))
			});

		QJsonDocument JsonDocument;
		QJsonObject obj;
		obj["config"] = config;
		obj["scandate"] = scandate;
		JsonDocument.setObject(obj);

		if (config_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
			config_file.write(JsonDocument.toJson());
			config_file.close();
		}
	}
}


void ConfigData::setConfigValue(QString key, bool opt)
{
	QJsonParseError JsonParseError;
	QJsonDocument JsonDocument;
	QFile file(_configfile_path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
		file.close();
	}
	else 
		JsonDocument = QJsonDocument::fromJson("", &JsonParseError);
	
	QJsonObject RootObject = JsonDocument.object();
	QJsonValueRef ref = RootObject.find("config").value();
	QJsonObject m_addvalue = ref.toObject();
	m_addvalue.insert(key, opt);//set the value you want to modify
	ref = m_addvalue; //assign the modified object to reference
	JsonDocument.setObject(RootObject); // set to json document

	if (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
		file.write(JsonDocument.toJson());
		file.close();
	}
}

void ConfigData::setAuthInfo()
{
	QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	QFile serialfile(QString("%1/serial").arg(appLocalPath));
	//QFile uuidfile(QString("%1/unique").arg(appLocalPath));

	_serial_key = "";
	if (serialfile.exists()) {
		QString serial("");
		if (serialfile.open(QIODevice::ReadWrite)) {
			QTextStream OpenSerialFile(&serialfile);

			while (!OpenSerialFile.atEnd())
			{
				serial = OpenSerialFile.readLine();
			}
			serialfile.close();

			_serial_key = serial;
		}
	}

	//if (uuidfile.exists()) {
	//	QString uuid("");
	//	if (uuidfile.open(QIODevice::ReadWrite)) {
	//		QTextStream OpenUUIDFile(&uuidfile);

	//		while (!OpenUUIDFile.atEnd())
	//		{
	//			uuid = OpenUUIDFile.readLine();
	//		}
	//		uuidfile.close();
	//	}

	//	_uuid = uuid;
	//}

}

void ConfigData::setStartupOpt(bool opt)
{
	if (_startup_opt == opt)
		return;

	//우선 제거
	/*if (opt == false) {
		if (SmartOptimizeInfo::inst()->getUseData() == true || AutoPcOffData::inst()->isAutoPcOffUse() == true ) {
			emit setLockStartupOpt();
			return;
		}
	}*/

	_startup_opt = opt;
	setConfigValue("startup", opt);

	setTaskSch(opt);

	//QString startuptmp = QString("\"%1\\NKeepCleanerStarter.exe\"").arg(qApp->applicationDirPath());
	//QString startupstr = startuptmp.replace("/", "\\");
	////utils::setStartup("N-KeepCleaner", startupstr, opt);
	//utils::setTaskScheduler("N-KeepCleanerStart", startupstr, opt);
	//utils::setStartup("N-KeepCleaner", qApp->applicationDirPath() + "/N-KeepCleaner.exe /hide", opt);
}

void ConfigData::setStartupOptOnly(bool opt)
{
	if (_startup_opt == opt)
		return;

	_startup_opt = opt;
	setConfigValue("startup", opt);
	if (opt == true)
		emit setChangeStartupOpt();

	setTaskSch(opt);

	//QString startuptmp = QString("\"%1\\NKeepCleanerStarter.exe\"").arg(qApp->applicationDirPath());
	//QString startupstr = startuptmp.replace("/", "\\");

	//utils::setStartup("N-KeepCleaner", startupstr, opt);
		
}
//
//void ConfigData::setRegOptimizeOpt(bool opt)
//{
//	if (_regoptimize_opt == opt)
//		return;
//
//	_regoptimize_opt = opt;
//	setConfigValue("regoptimize", opt);
//}
//
//bool ConfigData::getRegOptimizeOpt() const
//{
//	return _regoptimize_opt;
//}
//
//void ConfigData::setStartProgramOpt(bool opt)
//{
//	if (_start_program_opt == opt)
//		return;
//
//	_start_program_opt = opt;
//	setConfigValue("startprogram", opt);
//}
//
//bool ConfigData::getStartProgramOpt() const
//{
//	return _start_program_opt;
//}
//
//void ConfigData::setUninstallOpt(bool opt)
//{
//	if (_uninstall_opt == opt)
//		return;
//
//	_uninstall_opt = opt;
//	setConfigValue("uninstall", opt);
//}
//
//bool ConfigData::getUninstallOpt() const
//{
//	return _uninstall_opt;
//}

bool ConfigData::getStartupOpt() const
{
	return _startup_opt;
}

void ConfigData::setTrayOpt(bool opt)
{
	_tray_opt = opt;
	setConfigValue("tray", opt);
}

bool ConfigData::getTrayOpt() const
{
	return _tray_opt;
}

int ConfigData::getDiffDays() const 
{
	return _diff_days;
}

void ConfigData::updateScanDiffDay()
{
	if (_date.isEmpty() || _date == "0")
		_diff_days = -1;
	else {
		QDate nowdate = QDate::currentDate();
		QDate olddate = QDate::fromString(_date, "yyyy/MM/dd");
		_diff_days = olddate.daysTo(nowdate);
	}
}

void ConfigData::setTaskSch(bool use)
{
	//무조건 false(임시로)
	use = false;
	QString startuptmp = QString("\"%1\\NKeepCleanerStarter.exe\"").arg(qApp->applicationDirPath());
	QString startupstr = startuptmp.replace("/", "\\");
	//utils::setStartup("N-KeepCleaner", startupstr, opt);
	utils::setTaskScheduler("N-KeepCleanerStart", startupstr, use);

}

void ConfigData::updateOptimizeDate()
{
	QDateTime now = QDateTime::currentDateTime();
	_date = now.toString("yyyy/MM/dd");
	_time = now.toString("hh:mm:ss");
	_diff_days = 0;

	QJsonParseError JsonParseError;
	QJsonDocument JsonDocument;
	QFile file(_configfile_path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
		file.close();
	}
	else
		JsonDocument = QJsonDocument::fromJson("", &JsonParseError);

	QJsonObject RootObject = JsonDocument.object();
	QJsonValueRef ref = RootObject.find("scandate").value();
	QJsonObject m_addvalue = ref.toObject();
	m_addvalue.insert("date", _date);
	m_addvalue.insert("time", _time);
	ref = m_addvalue; 
	JsonDocument.setObject(RootObject); // set to json document

	if (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
		file.write(JsonDocument.toJson());
		file.close();
	}

	emit setScanDiffDay();
}

void ConfigData::setAuthInfo(QString serial, QString uuid)
{
	_serial_key = serial;
	_uuid = uuid;

	QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	if (QDir(appLocalPath).exists() == false) {
		QDir().mkdir(appLocalPath);
	}

	QFile serialfile(QString("%1/serial").arg(appLocalPath));
	if (serialfile.open(QIODevice::ReadWrite)) {
		QTextStream stream(&serialfile);
		stream << serial;
		serialfile.close();
	}

	QFile uniquefile(QString("%1/unique").arg(appLocalPath));
	if (uniquefile.open(QIODevice::ReadWrite)) {
		QTextStream stream(&uniquefile);
		stream << uuid;
		uniquefile.close();
	}


}

void ConfigData::clearAuthinfo()
{
	_serial_key = _uuid = "";

	QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));

	QFile serialfile(QString("%1/serial").arg(appLocalPath));
	if (serialfile.exists()) {
		serialfile.remove();
	}

	QFile uniquefile(QString("%1/unique").arg(appLocalPath));
	if (uniquefile.exists()) {
		uniquefile.remove();
	}

}

void ConfigData::setSerialKey(QString serial)
{
	_serial_key = serial;

	QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	if (QDir(appLocalPath).exists() == false) {
		QDir().mkdir(appLocalPath);
	}

	QFile serialfile(QString("%1/serial").arg(appLocalPath));
	if (serialfile.open(QIODevice::ReadWrite)) {
		QTextStream stream(&serialfile);
		stream << serial;
		serialfile.close();
	}

}

void ConfigData::setUUID(QString uuid)
{
	_uuid = uuid;

	QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	if (QDir(appLocalPath).exists() == false) {
		QDir().mkdir(appLocalPath);
	}

	QFile uniquefile(QString("%1/unique_n").arg(appLocalPath));
	if (uniquefile.open(QIODevice::ReadWrite)) {
		QTextStream stream(&uniquefile);
		stream << uuid;
		uniquefile.close();
	}

}

//예전 버전의 저장된 uuid 
QString ConfigData::getOldFileUUID()
{
	QString oldUuid("");
	QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	QFile uuidfile(QString("%1/unique").arg(appLocalPath));
	if (uuidfile.exists()) {
		QString uuid("");
		if (uuidfile.open(QIODevice::ReadWrite)) {
			QTextStream OpenUUIDFile(&uuidfile);

			while (!OpenUUIDFile.atEnd())
			{
				uuid = OpenUUIDFile.readLine();
			}
			uuidfile.close();
		}

		oldUuid = uuid;
	}

	return oldUuid;
}
