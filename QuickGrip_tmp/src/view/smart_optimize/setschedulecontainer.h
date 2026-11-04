#pragma once

#include <QWidget>
#include <QList>

#include "view/Component/button.h"
#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"

#include "../../model/smart_optimize/smartopimizedata.h"

#include <common/defines/defineproperty.h>

#include "combowidget.h"

class SetScheduleContainer  : public QWidget
{
	Q_OBJECT

		PROPERTY(bool, notuse, setNotUse)

public:
	SetScheduleContainer(QWidget *parent = nullptr);
	~SetScheduleContainer();

private:
	bool _pcoff;
	Button* _day;
	Button* _week;
	Button* _month;

	Button* _mon;
	Button* _tues;
	Button* _wednes;
	Button* _thurs;
	Button* _fri;
	Button* _sat;
	Button* _sun;

	Button* _firstmon;
	Button* _fifteenmon;
	Button* _thirtymon;

	LastClieckType _lastselect_index;

	ReservWeekType _weektype;
	ReservMonthType _monthtype;

	bool _firstrun_week;
	bool _firstrun_month;


	ComboWidget* _combowidget;
	QLabel* _summary;
	CustomLabel* _summary_label;
	CustomLabel* _summary_typelabel;
	Button* _insertbtn;


	void enableWeekButton(bool enable);
	void enableMonthButton(bool enable);

	void updateSummaryTypeLabel();

	void updateWeekTypes(ReservWeekType type);
	void updateMonthTypes(ReservMonthType type);


	ReserveData getReserveData();

public:
	void setPcAutoOffType(bool pcoff) { _pcoff = pcoff; }
	QString getStringbyWeekType(ReservWeekType type);
	QString getStringbyMonthType(ReservMonthType type);

	void notuseUpdate();
	void useUiUpdate();

signals:
	void insertResve(QString text, ReserveData data);
	void insertPcOff(ReserveData data);

	void updatetime(int hour, int min);
	void buttonNotUseAlert();

public slots:
	void updateReserveData(ReserveData data);

};
