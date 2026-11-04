#pragma once

#include <QObject>
#include <QPointer>


#include <common/tool/ProcessDetector.h>
#include <common/ux/singleton.h>

class CommonController  : public QObject, 
	public common::ux::Singleton<CommonController>
{
	Q_OBJECT

public:
	CommonController(QObject *parent = Q_NULLPTR);
	~CommonController();


private:
	QPointer<ProcessDetector> _processchecker;



Q_SIGNALS:
	void nortonProcessChecking();

public Q_SLOTS:
	void processcheckRun();

	void processChecked();
};
