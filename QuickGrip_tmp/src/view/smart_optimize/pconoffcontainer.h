#pragma once

#include <QObject>
#include <QWidget>
#include <QPointer>

#include "view/Component/vlayout.h"
#include "view/Component/button.h"

class PcOnOffContainer  : public QWidget
{
	Q_OBJECT

public:
	PcOnOffContainer(bool ispcon, bool ispcoff, QWidget*parent = nullptr);
	~PcOnOffContainer();

	void setUncheck(bool pcoff);
	void setUncheckAll();

private:
	QPointer<Button> _pcon;
	//QPointer<Button> _pcoff;

signals:
	void sendPcOnOff(bool pcon);
};
