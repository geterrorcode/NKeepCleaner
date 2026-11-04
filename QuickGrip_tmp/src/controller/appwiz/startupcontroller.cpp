#include "appwizcontroller.h"

#include "../../../QuickGrip_tmp.h"
#include "../../view/appwiz/appwizview.h"
#include "../../model/appwiz/appwizmanager.h"



AppwizController::AppwizController(QObject *parent)
	: QObject(parent)
{
}

AppwizController::~AppwizController()
{}


void AppwizController::slotgetAppwizData()
{
	AppwizManager::inst()->updateAppWizData();

	//emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
}

void AppwizController::slotdelAppwizData(int key)
{
	AppwizManager::inst()->delAppwizData(key);
	
	//emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
}