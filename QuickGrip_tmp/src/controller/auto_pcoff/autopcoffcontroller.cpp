#include "autopcoffcontroller.h"


#include <model/auto_pcoff/autopcoffchecker.h>

#include <common/tool/utils.h>

AutoPcOffController::AutoPcOffController(QObject *parent)
	: QObject(parent)
{
	//bool use = AutoPcOffData::inst()->isAutoPcOffUse();

	connect(AutoPcOffData::inst(), &AutoPcOffData::callPcOffAlert, [=]() {
		emit pcOffTrayPopup();
		});

	connect(AutoPcOffData::inst(), &AutoPcOffData::callPCOff, [=]() {
		utils::pcShutDown();
		});
}

AutoPcOffController::~AutoPcOffController()
{}

void AutoPcOffController::pcoffcheckTimer()
{

}

bool AutoPcOffController::isUse() const
{
	return AutoPcOffData::inst()->isAutoPcOffUse();
}

void AutoPcOffController::receiveData()
{
	ReserveData data = AutoPcOffData::inst()->getPcoffData();
	emit updatePcOffViewData(data);
	
}

void AutoPcOffController::setPcOffData(ReserveData data)
{
	AutoPcOffData::inst()->setPcOffData(data);
	//emit updatePcOffViewData(data);
	emit updateOK();
}

void AutoPcOffController::setPcOffConfigDisable(bool disable)
{
	AutoPcOffData::inst()->setPcOffOnOff(disable);

	emit changeSetting(disable);
}