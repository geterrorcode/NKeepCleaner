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

void AppwizController::appwizAlertCaller()
{
	emit signalAppwizAlertCall();
}

void AppwizController::slotgetAppwizData()
{
	AppwizManager::inst()->updateAppWizData();

	emit signalsetAppwizData(AppwizManager::inst()->getFirstIter(), AppwizManager::inst()->getLastIter());
}

void AppwizController::slotdelAppwizData(int key)
{
	AppwizManager::inst()->delAppwizData(key);
	
	//emit signalsetStartUpData(StartupSearcher::inst()->getFirstIter(), StartupSearcher::inst()->getLastIter());
}

void AppwizController::slotdelResult(int key)
{
	emit signaldelAppwizdataResult(key);
}