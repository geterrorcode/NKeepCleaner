#pragma once

#include <QWidget>
#include <QLabel>
#include <QWheelEvent>
#include <QPointer>

#include "combocontainer.h"

#include "view/Component/button.h"
#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"

class ComboWidget  : public QWidget
{
	Q_OBJECT

public:
	ComboWidget(QWidget *parent = Q_NULLPTR);
	~ComboWidget();

protected:
	void wheelEvent(QWheelEvent* event) override;

private:
	//CustomLabel* _hour;
	//CustomLabel* _min;
	int _combohour;
	int _combomin;

	void updateTime();

	QPointer<ComboContainer> _hourcombo;
	QPointer<ComboContainer> _mincombo;

public:
	int getHour() { return _combohour; };
	int getMin() { return _combomin; };


signals:
	void sendSetTime(QString settime);

	void setHour(int hour);
	void setMin(int min);

public slots:
	void setComboTime(int hour, int min);

};
