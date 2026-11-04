#pragma once

#include <QObject>
#include "../../common/ux/singleton.h"

class PcinfoManager  : public QObject, public common::ux::Singleton<PcinfoManager>
{
	Q_OBJECT

public:
	PcinfoManager(QObject *parent = Q_NULLPTR);
	~PcinfoManager();


public:
	void updatePcInfos();

	const QString getOS() { return _osinfo; }
	const QString getCPU() { return _cpuinfo; }
	const QString getRam() { return _raminfo; }
	const QString getGPU() { return _gpuinfo; }
	const QString getDisplay() { return _displayinfo; }
	const QString getIP() { return _ipaddress; }
	const QString getMac() { return _macaddress; }

private:
	QString _osinfo;
	QString _cpuinfo;
	QString _raminfo;
	QString _gpuinfo;
	QString _displayinfo;
	QString _default_browser;
	QString _ipaddress;
	QString _macaddress;

private:
	QString getOSInfo();
	QString getCPUInfo();
	QString getRamInfo();
	QString getGPUInfo();
	QString getDisplayInfo();
	//QString getDefaultBrowserInfo(const int ostype);
	QString getMacAddress();
	QString getIPAddress();
	
	//std::tuple<QString, QString> getIPandMacaddress();

	

};
