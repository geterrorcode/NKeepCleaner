#include "combocontainer.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

ComboContainer::ComboContainer(int min, int max, int defaultvalue, QWidget *parent)
	: QWidget(parent), _min(min), _max(max), _value(defaultvalue), _label(Q_NULLPTR), _upbtn(Q_NULLPTR), _downbtn(Q_NULLPTR)
{

	setFixedSize(40, 98);

	_label = new CustomLabel(16, "#ffffff", true, 0, 0, this);
	_label->setText(QStringLiteral("%1").arg(_value, 2, 10, QLatin1Char('0')));
	_label->setGeometry(7, 40, 24, 24);
	_label->show();

	_upbtn = new Button(this);
	_upbtn->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/comboup.png"));
	_upbtn->setGeometry(15, 10, 10, 10);
	_upbtn->show();
	connect(_upbtn, &Button::clicked, [=]() {
		clickedup();
		/*_value++;
		if (_value > _max)
			_value = _min;
		_upbtn->setText(QStringLiteral("%1").arg(_value, 2, 10, QLatin1Char('0')));*/
		//updateTime();
		});

	_downbtn = new Button(this);
	_downbtn->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/combodown.png"));
	_downbtn->setGeometry(15, 80, 10, 10);
	_downbtn->show();
	connect(_downbtn, &Button::clicked, [=]() {
		clickeddown();
		//updateTime();
		});



}

ComboContainer::~ComboContainer()
{}



void ComboContainer::wheelEvent(QWheelEvent* event)
{
	// You can use event->angleDelta() to get the distance the wheel was rotated.
	int delta = event->angleDelta().y();

	if (delta > 0)
	{
		clickedup();
	}
	else if (delta < 0)
	{
		clickeddown();
		// Wheel scrolled down
	}

	// Don't forget to call the base class implementation
	QWidget::wheelEvent(event);
}

void ComboContainer::clickedup()
{
	_value++;
	if (_value > _max)
		_value = _min;
	_label->setText(QStringLiteral("%1").arg(_value, 2, 10, QLatin1Char('0')));
	emit sendvalue(_value);
}

void ComboContainer::clickeddown()
{
	_value--;
	if (_value < _min)
		_value = _max;
	_label->setText(QStringLiteral("%1").arg(_value, 2, 10, QLatin1Char('0')));
	emit sendvalue(_value);
}

void ComboContainer::setComboValue(int value)
{
	_value = value;
	_label->setText(QStringLiteral("%1").arg(_value, 2, 10, QLatin1Char('0')));
}