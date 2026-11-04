#include "nlog.h"

#include <QDateTime>
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QNetworkSession>
#include <QDesktopWidget>
#include <QApplication>

#include "Network/apirequest.h"
#include "Util/Util.h"
#include "env/env.h"


NLog::NLog()
{
	_macAddress = "Unknown";
	_os = "Unknown";
	_cpu = "Unknown";
	_ram = "Unknown";
	_resolution = "Unknown";
	_defaultBrowser = "Unknown";
	_ipAddress = "Unknown";

}

void NLog::initialize()
{
	auto result =  getNetwork();
	_ipAddress = std::get<0>(result);
	_macAddress = std::get<1>(result);

	getSystemInfo();
}


void NLog::getSystemInfo()
{
	{//os
		QString szTxt = QSysInfo::prettyProductName();
		szTxt.append(QString(", %1").arg(QSysInfo::kernelVersion()));
		std::string platform = Util::Environment::getPlatform();
		QString osBit(platform.c_str());
		szTxt.append(",");
		szTxt.append(osBit);

		_os = szTxt;
	}
	{//cpu
		int nProcCount = 0;
		QString szTxt;

		QSettings setProc(QString("HKEY_LOCAL_MACHINE\\Hardware\\DESCRIPTION\\System\\CentralProcessor"), QSettings::NativeFormat);
		nProcCount = setProc.childGroups().count();
		if (nProcCount > 0)
		{
			QSettings set(QString("HKEY_LOCAL_MACHINE\\Hardware\\DESCRIPTION\\System\\CentralProcessor\\0"), QSettings::NativeFormat);

			QStringList szInfo;
			szInfo.append(QString("%1").arg(set.value("ProcessorNameString").toString()));
			szInfo.append(QString("%1 MHz").arg(set.value("~MHz").toUInt()));
			szInfo.append(QString("#%1 Procs").arg(nProcCount));

			szTxt = szInfo.join(", ");

			
		}
		else//error
		{
			szTxt = QString("Architecture:%1, Identifier=%2, Level=%3, Revision=%4").arg(getenv("PROCESSOR_ARCHITECTURE")).arg(getenv("PROCESSOR_IDENTIFIER")).arg(getenv("PROCESSOR_LEVEL")).arg(getenv("PROCESSOR_REVISION"));
		}

		_cpu = szTxt;
	}
	{//ram
		MEMORYSTATUSEX memory_status;
		ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
		memory_status.dwLength = sizeof(MEMORYSTATUSEX);
		QString szTxt("");
		if (GlobalMemoryStatusEx(&memory_status))
			szTxt = QString("%1 MB").arg(memory_status.ullTotalPhys / (1024 * 1024));

		_ram = szTxt;
	}

	{//resolution
		
		auto  widthNumber = QApplication::desktop()->screenGeometry().width();
		auto  heightNumber = QApplication::desktop()->screenGeometry().height();

		_resolution = QString("%1 x %2").arg(widthNumber).arg(heightNumber);

	}
	
	{ //Default Browser
		_defaultBrowser = getDefaultBrowserInfo((int)QSysInfo::windowsVersion());
	}

}

QString NLog::getDefaultBrowserInfo(const int ostype)
{
	QString browserName("Unknown");
	QString defaultIEReg, progId;

	if (ostype == QSysInfo::WV_WINDOWS7)
	{
		defaultIEReg = "HKEY_CLASSES_ROOT\\.html";
		progId = ".";
	}
	else //win10
	{
		defaultIEReg = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.html\\UserChoice";
		progId = "ProgId";
	}

	QSettings setDefaultIEReg(QString(defaultIEReg), QSettings::NativeFormat);
	QStringList szInfo;
	QString userSet(QString("%1").arg(setDefaultIEReg.value(progId).toString()));

	if (userSet == "htmlfile" || userSet.isEmpty()) {
		browserName = "Internet Explorer";
	}
	else {
		QSettings setIeNameReg(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\%1\\Application").arg(userSet), QSettings::NativeFormat);
		browserName = QString("%1").arg(setIeNameReg.value("ApplicationName").toString());
		if (browserName.isEmpty()) //Firefox는 방식이 다르다
		{
			//보조 레지스트리에서 한번더 검색
			QSettings setIeNameSubReg(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Clients\\StartMenuInternet\\%1\\Capabilities").arg(userSet), QSettings::NativeFormat);

			browserName = QString("%1").arg(setIeNameSubReg.value("ApplicationName").toString());

			if (browserName.isEmpty()) //최종적으로 판단 안되는 항목은 문자열 파싱으로 전달
			{
				QString buf(QString(userSet.left(userSet.indexOf("HTML", Qt::CaseInsensitive)))); //FirefoxHTML-308046B0AF4A39CB 형태
				browserName = buf;
			}
		}
	}

	return browserName;
}

//QString NLog::getIpAddress()
//{
//	QString ipAddress;
//	foreach(const QNetworkInterface & netInterface, QNetworkInterface::allInterfaces()) {
//		QNetworkInterface::InterfaceFlags flags = netInterface.flags();
//		if ((bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)) {
//			foreach(const QNetworkAddressEntry & address, netInterface.addressEntries()) {
//				if (address.ip().protocol() == QAbstractSocket::IPv4Protocol)
//					ipAddress = address.ip().toString();
//			}
//		}
//	}
//
//	return ipAddress;
//}

std::tuple<QString, QString> NLog::getNetwork()
{
	QString ipAddress, macAddress;
	QNetworkConfiguration nc;
	QNetworkConfigurationManager ncm;
	QList<QNetworkConfiguration> configsForEth, configsForWLAN, allConfigs;
	foreach(nc, ncm.allConfigurations(QNetworkConfiguration::Active))
	{
		if (nc.type() == QNetworkConfiguration::InternetAccessPoint)
		{
			if (nc.bearerType() == QNetworkConfiguration::BearerWLAN)
			{
				configsForWLAN.append(nc);
			}
			if (nc.bearerType() == QNetworkConfiguration::BearerEthernet)
			{
				configsForEth.append(nc);
			}
		}
	}


	
	allConfigs.append(configsForWLAN);
	allConfigs.append(configsForEth);

	foreach(nc, allConfigs)
	{
		QNetworkSession networkSession(nc);
#undef interface
		QNetworkInterface netInterface = networkSession.interface();
		if (!(netInterface.flags() & QNetworkInterface::IsLoopBack)
			&& !netInterface.humanReadableName().toLower().contains("vmware")
			&& !netInterface.humanReadableName().toLower().contains("virtual"))
		{
			macAddress = QString(netInterface.hardwareAddress());
			foreach(const QNetworkAddressEntry & address, netInterface.addressEntries()) {
				if (address.ip().protocol() == QAbstractSocket::IPv4Protocol){
					ipAddress = address.ip().toString();
					qDebug() << ipAddress;
				}
			}

			break;
		}
	}


	if (macAddress.isEmpty() == true)
	{
		foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
		{
			// Return only the first non-loopback MAC Address
			if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
			{
				macAddress = netInterface.hardwareAddress();
				
				foreach(const QNetworkAddressEntry & address, netInterface.addressEntries()) {
					if (address.ip().protocol() == QAbstractSocket::IPv4Protocol){
						ipAddress = address.ip().toString();
					}
				}

				break;
			}
		}

	}

	return std::tie(ipAddress, macAddress);


}

const QString& NLog::getMacAddress()
{
	return _macAddress;
}

const QString& NLog::getIpAddress()
{
	return _ipAddress;
}

const QString& NLog::getOS()
{
	return _os;
}

const QString& NLog::getRAM()
{
	return _ram;
}

const QString& NLog::getCPU()
{
	return _cpu;
}

const QString& NLog::getResolution()
{
	return _resolution;
}

const QString& NLog::getDefaultBrowser()
{
	return _defaultBrowser;
}

QJsonObject NLog::CommonData(const int subCategory, const QJsonObject& dynamicJsonData)
{
	QJsonObject commonNlog;

	QDateTime local(QDateTime::currentDateTime());
	QDateTime UTC(local.toTimeSpec(Qt::UTC));
	QString currentTime = UTC.toString(Qt::ISODateWithMs);
	
	currentTime.replace("T", " ");
	currentTime.replace("Z", "");

	commonNlog["game_code"] = "";
	commonNlog["server_id"] = 0;
	commonNlog["caller"] = 0;
	commonNlog["current_time"] = currentTime;
	commonNlog["player_id"] = "";
	commonNlog["user_seq"] = 0;
	commonNlog["channel_id"] = 0;
	commonNlog["map_id"] = 0; 
	commonNlog["world_id"] = 0;
	commonNlog["guild_id"] = 0;
	commonNlog["character_tid"] = 0;
	commonNlog["play_time"] = 0;
	commonNlog["player_level"] = 0;
	commonNlog["character_level"] = 0;
	commonNlog["fighting_power"] = 0;
	commonNlog["correlation_id"] = "";
	commonNlog["log_type"] = 1;
	commonNlog["category"] = 80; 
	commonNlog["sub_category"] = subCategory;

	commonNlog.insert("dynamic_log", dynamicJsonData);
	

	return commonNlog;

}
#include <QJsonDocument>

void NLog::send(const int subCategory, const QJsonObject& dynamicJsonData)
{
	QString logUrl = QString("%1/gameLog").arg(env().nlogHost);

	

	QJsonObject sendObj = CommonData(subCategory, dynamicJsonData);

	Request::API::post(nullptr, QUrl(logUrl), sendObj,
		[=](int statusCode, QVariantMap result) {
			if (result["result"].toInt() != 0) return;
	});

}

