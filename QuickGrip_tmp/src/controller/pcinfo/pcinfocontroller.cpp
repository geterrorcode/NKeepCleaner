#include "pcinfocontroller.h"


#include "../../model/pcinfo/pcinfomanager.h"

PcInfoController::PcInfoController(QObject *parent)
	: QObject(parent)
{
	PcinfoManager::inst()->updatePcInfos();
}

PcInfoController::~PcInfoController()
{}

const QString PcInfoController::getOSInfo()
{
	return PcinfoManager::inst()->getOS();
}

const QString PcInfoController::getCPUInfo()
{
	return PcinfoManager::inst()->getCPU();
}

const QString PcInfoController::getRamInfo()
{
	return PcinfoManager::inst()->getRam();
}

const QString PcInfoController::getGPUInfo()
{
	return PcinfoManager::inst()->getGPU();
}

const QString PcInfoController::getDisplayInfo()
{
	return PcinfoManager::inst()->getDisplay();
}

const QString PcInfoController::getMacAddress()
{
	return PcinfoManager::inst()->getMac();
}

const QString PcInfoController::getIPAddress()
{
	return PcinfoManager::inst()->getIP();
}