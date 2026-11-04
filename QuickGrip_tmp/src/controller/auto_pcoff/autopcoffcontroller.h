#pragma once

#include <QObject>

#include "../../../QuickGrip_tmp.h"

#include <view/Component/traypopupcomponent.h>

#include <model/auto_pcoff/autopcoffdata.h>


#include "../../common/ux/singleton.h"

class AutoPcOffController  : public QObject, public common::ux::Singleton<AutoPcOffController>
{
	Q_OBJECT

public:
	AutoPcOffController(QObject *parent = Q_NULLPTR);
	~AutoPcOffController();

private:
	

	void pcoffcheckTimer();

public:
	bool isUse() const;


Q_SIGNALS:
	void updatePcOffViewData(ReserveData data);
	void updateOK();
	void pcOffTrayPopup();
	void changeSetting(bool use);

public Q_SLOTS:

	void receiveData();
	void setPcOffData(ReserveData data);
	void setPcOffConfigDisable(bool disable);
	//void getPcOffData();
	
};
