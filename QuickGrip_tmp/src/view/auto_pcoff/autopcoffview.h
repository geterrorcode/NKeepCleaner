#pragma once

#include <QWidget>
#include <QPointer>
#include <QCheckBox>

#include <model/smart_optimize/smartopimizedata.h>


class AutoPcOffView  : public QWidget
{
	Q_OBJECT

public:
	AutoPcOffView(QWidget *parent);
	~AutoPcOffView();

private:
	QPointer<QCheckBox> _usecheck;
	QPointer<QCheckBox> _offcheck;


Q_SIGNALS:
	void getPcOffReserveData();
	void updatePcOffData(ReserveData data);
	void updatePcOffReserveData(ReserveData data);
	void PcOffOptionOnOff(bool off);



};
