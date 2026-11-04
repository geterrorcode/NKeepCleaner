#pragma once

#include <QWidget>
#include <QWheelEvent>

#include <view/Component/button.h>
#include <view/Component/customlabel.h>

class ComboContainer  : public QWidget
{
	Q_OBJECT

public:
	ComboContainer(int min = 0, int max = 0, int defaultvalue = 0, QWidget *parent = Q_NULLPTR);
	~ComboContainer();

protected:
	void wheelEvent(QWheelEvent* event) override;

private:
	int _min;
	int _max;
	int _value;

	CustomLabel* _label;
	Button *_upbtn;
	Button *_downbtn;

	void clickedup();
	void clickeddown();


Q_SIGNALS:
	void sendvalue(int value);


public Q_SLOTS:
	void setComboValue(int value);
};
