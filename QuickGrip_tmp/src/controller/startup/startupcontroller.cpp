#include "startupcontroller.h"

#include "../../../QuickGrip_tmp.h"
#include "../../view/startup/startupview.h"
#include "../../model/startup/StartupSearcher.h"



StartupController::StartupController(QObject *parent)
	: QObject(parent)
{
	//QObject::connect(this, SIGNAL(signalgetStartUpData()), StartupSearcher::inst(), SLOT(slotsearStartUp()));
	//QObject::connect(this, SIGNAL(void signalsetStartUpData()), QuickGrip_tmp::inst(), SLOT(slotgetStartupData()));
}

StartupController::~StartupController()
{}


void StartupController::slotgetStartupData()
{
	StartupSearcher::inst()->updateStartUp();

	//emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
	emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
}

void StartupController::getStartupData()
{
	StartupSearcher::inst()->updateStartUp();

	//emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
}

void StartupController::slotdelStartup(QList<int> keys)
{
	StartupSearcher::inst()->deleteStartUpList(keys);

	//for (auto key : keys)
	//{
	//	StartupSearcher::inst()->delStartUp(key);
	//}
	emit signalsetStartDelComplete();
	emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
}