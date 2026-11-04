#include "combowidget.h"



#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

ComboWidget::ComboWidget(QWidget *parent)
	: QWidget(parent), _hourcombo(Q_NULLPTR), _mincombo(Q_NULLPTR)
{
	_combohour = 12; 
	_combomin = 0;

	setFixedSize(124, 98);

	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/gbcombo.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	_combohour = 12;
	//auto hourcombo = new ComboContainer(1, 24, _combohour, this);
	_hourcombo = new ComboContainer(0, 23, _combohour, this);
	_hourcombo->setGeometry(0, 0, 40, 98);
	_hourcombo->show();

	_combomin = 0;
	//auto mincombo = new ComboContainer(0, 59, _combomin, this);
	_mincombo = new ComboContainer(0, 59, _combomin, this);
	_mincombo->setGeometry(80, 0, 40, 98);
	_mincombo->show();

	//auto hour_up = new Button(this);
	//hour_up->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/comboup.png"));
	//hour_up->setGeometry(15, 10, 10, 10);
	//hour_up->show();
	//connect(hour_up, &Button::clicked, [=]() {
	//	_combohour++;
	//	if (_combohour > 24)
	//		_combohour = 1;
	//	_hour->setText(QStringLiteral("%1").arg(_combohour, 2, 10, QLatin1Char('0')));
	//	updateTime();
	//	});

	//auto hour_down = new Button(this);
	//hour_down->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/combodown.png"));
	//hour_down->setGeometry(15, 80, 10, 10);
	//hour_down->show();
	//connect(hour_down, &Button::clicked, [=]() {
	//	_combohour--;
	//	if (_combohour < 1)
	//		_combohour = 24;
	//	_hour->setText(QStringLiteral("%1").arg(_combohour, 2, 10, QLatin1Char('0')));
	//	updateTime();
	//	});


	//auto min_up = new Button(this);
	//min_up->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/comboup.png"));
	//min_up->setGeometry(100, 10, 10, 10);
	//min_up->show();
	//connect(min_up, &Button::clicked, [=]() {
	//	_combomin++;
	//	if (_combomin > 59)
	//		_combomin = 0;
	//	_min->setText(QStringLiteral("%1").arg(_combomin, 2, 10, QLatin1Char('0')));
	//	updateTime();
	//	});

	//auto min_down = new Button(this);
	//min_down->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/combodown.png"));
	//min_down->setGeometry(100, 80, 10, 10);
	//min_down->show();
	//connect(min_down, &Button::clicked, [=]() {
	//	_combomin--;
	//	if (_combomin < 0)
	//		_combomin = 59;
	//	_min->setText(QStringLiteral("%1").arg(_combomin, 2, 10, QLatin1Char('0')));
	//	updateTime();
	//	});
	
	/*_hour = new CustomLabel(14, "#ffffff", true, 0, 0, this);
	
	_hour->setText(QStringLiteral("%1").arg(_combohour, 2, 10, QLatin1Char('0')));
	_hour->setGeometry(7, 40, 24, 24);
	_hour->show();*/

	auto dot = new CustomLabel(14, "#ffffff", true, 0, 0, this);
	dot->setText(":");
	dot->setGeometry(60, 40, 20, 20);
	dot->show();

	/*_min = new CustomLabel(14, "#ffffff", true, 0, 0, this);
	_min->setText(QStringLiteral("%1").arg(_combomin, 2, 10, QLatin1Char('0')));
	_min->setGeometry(92, 40, 24, 24);
	_min->show();*/


	QObject::connect(_hourcombo, &ComboContainer::sendvalue, [=](int value) {
		_combohour = value;
		updateTime();
		});

	QObject::connect(_mincombo, &ComboContainer::sendvalue, [=](int value) {
		_combomin = value;
		updateTime();
		});


	QObject::connect(this, SIGNAL(setHour(int)), _hourcombo.get(), SLOT(setComboValue(int)));
	QObject::connect(this, SIGNAL(setMin(int)), _mincombo.get(), SLOT(setComboValue(int)));

}

ComboWidget::~ComboWidget()
{}


void ComboWidget::updateTime()
{
	QString settime;
	settime = QString("%1 : %2").arg(QStringLiteral("%1").arg(_combohour, 2, 10, QLatin1Char('0'))).arg(QStringLiteral("%1").arg(_combomin, 2, 10, QLatin1Char('0')));
	emit sendSetTime(settime);
}

void ComboWidget::wheelEvent(QWheelEvent* event)
{
	// You can use event->angleDelta() to get the distance the wheel was rotated.
	
	// Don't forget to call the base class implementation
	QWidget::wheelEvent(event);
}

void ComboWidget::setComboTime(int hour, int min)
{
	_combohour = hour;
	_combomin = min;
	emit setHour(hour);
	emit setMin(min);
}