#include "pcinfomanager.h"

#include <windows.h>

#include <QDateTime>
#include <QNetworkInterface>
#include <QNetworkInformation>
#include <QSettings>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QSysInfo>



#define _WIN32_DCOM
#include <iostream>
#include <tchar.h>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib,"user32.lib")

# pragma comment(lib, "wbemuuid.lib")

#include "../../common/tool/utils.h"
PcinfoManager::PcinfoManager(QObject *parent)
	: QObject(parent), 	_osinfo(""), _cpuinfo(""), _raminfo(""), _gpuinfo(""), _displayinfo(""),
	_default_browser(""), _ipaddress(""), _macaddress("")
{


}

PcinfoManager::~PcinfoManager()
{}


void PcinfoManager::updatePcInfos()
{
	_osinfo = getOSInfo();
	_cpuinfo = getCPUInfo();
	_raminfo = getRamInfo();
	_gpuinfo = getGPUInfo();
	_displayinfo = getDisplayInfo();
	_macaddress = getMacAddress();
	_ipaddress = getIPAddress();

}

QString PcinfoManager::getOSInfo()
{
	QString szTxt = QSysInfo::prettyProductName();
	szTxt.append(QString(", %1").arg(QSysInfo::kernelVersion()));
	std::string platform = utils::getPlatform();
	QString osBit(platform.c_str());
	szTxt.append(",");
	szTxt.append(osBit);

	return szTxt;
}

QString PcinfoManager::getCPUInfo()
{
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

	return szTxt;
}

QString PcinfoManager::getRamInfo()
{
	MEMORYSTATUSEX memory_status;
	ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
	memory_status.dwLength = sizeof(MEMORYSTATUSEX);
	QString szTxt("");
	if (GlobalMemoryStatusEx(&memory_status))
		return utils::getFileSizeType(memory_status.ullTotalPhys);
		//szTxt = QString("%1 MB").arg(memory_status.ullTotalPhys / (1024 * 1024));

	return szTxt;
}

QString PcinfoManager::getGPUInfo()
{
	QString gpu_name = "UnKnown";

	for (int i = 0; ; i++)
	{
		DISPLAY_DEVICE dd = { sizeof(dd), 0 };
		BOOL f = EnumDisplayDevices(NULL, i, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
		if (!f)
			break;

		gpu_name = QString("%1").arg(dd.DeviceString);
	}

	return gpu_name;
}

QString PcinfoManager::getDisplayInfo()
{
	size_t width = 0;
	size_t heigth = 0;

	// Get the primary screen
	QScreen* screen = QApplication::primaryScreen();

	// Get the physical resolution of the screen
	QSize resolution = screen->geometry().size();

	size_t sizex = resolution.width();
	size_t sizey = resolution.height();
	qreal scaleFactor = screen->devicePixelRatio();

	if (scaleFactor > 1) { 
		width = sizex + (sizex * (scaleFactor - 1));
		heigth = sizey + (sizey * (scaleFactor - 1));
	}
	else if (scaleFactor < 1) {
		width = sizex - (sizex * (1 - scaleFactor));
		heigth = sizey - (sizey * (1 - scaleFactor));
	}
	else { //100%
		width = sizex;
		heigth = sizey;
	}
	

	return QString("%1 x %2").arg(width).arg(heigth);
}

QString PcinfoManager::getMacAddress()
{

	foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
	{
		if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) &&
			(netInterface.flags() & QNetworkInterface::IsUp) &&
			(netInterface.flags() & QNetworkInterface::IsRunning)) {

			
			QString macname = QString("(%1) %2").arg(netInterface.humanReadableName()).arg(netInterface.hardwareAddress());
			return macname;
			//return netInterface.hardwareAddress();
		}

		//if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
		//	return netInterface.hardwareAddress();
	}

	return "Unknown";
}

QString PcinfoManager::getIPAddress()
{
	const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
	for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
			return address.toString();
	}

	return "Unknown";
}
