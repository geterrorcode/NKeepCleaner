#include "commoncontroller.h"

#include <QDebug>

CommonController::CommonController(QObject *parent)
	: QObject(parent) , _processchecker(Q_NULLPTR)
{

	if (!_processchecker)
		_processchecker = new ProcessDetector(L"N360Downloader.exe", this);

	//QObject::connect(_processchecker.get(), SIGNAL(sendProcessChecked()), this, SLOT(processChecked()));
	QObject::connect(_processchecker.get(), &ProcessDetector::sendProcessChecked, [=]() {

		qDebug() << "Checking!";
		emit nortonProcessChecking();
	    });


}

CommonController::~CommonController()
{
	if (_processchecker)
		_processchecker->deleteLater();
	_processchecker = Q_NULLPTR;
}


void CommonController::processcheckRun()
{
	//if(!_processchecker)
	//	_processchecker = new ProcessDetector(L"N360Downloader.exe", this);

}


void CommonController::processChecked()
{
	_processchecker->deleteLater();
	_processchecker = Q_NULLPTR;
}