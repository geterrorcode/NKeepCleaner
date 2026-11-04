#pragma once

#include <QObject>

#include "../../common/ux/singleton.h"

class PcInfoController  : public QObject
	, public common::ux::Singleton<PcInfoController>
{
	Q_OBJECT

public:
	PcInfoController(QObject *parent = Q_NULLPTR);
	~PcInfoController();


	const QString getOSInfo();
	
	const QString getCPUInfo();
	const QString getRamInfo();
	const QString getGPUInfo();
	const QString getDisplayInfo();
	const QString getMacAddress();
	const QString getIPAddress();
};
