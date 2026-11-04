#include "setschedulecontainer.h"

#include <qobject.h>
#include <QComboBox>
#include <QAbstractItemView>
#include <QWidget>

#include "view/Component/hlayout.h"

#include "view/Component/customlabel.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

SetScheduleContainer::SetScheduleContainer(QWidget* parent)
	: QWidget(parent), _day(Q_NULLPTR), _week(Q_NULLPTR), _month(Q_NULLPTR), _combowidget(Q_NULLPTR)
	, _summary(Q_NULLPTR), _summary_label(Q_NULLPTR), _summary_typelabel(Q_NULLPTR), _insertbtn(Q_NULLPTR)
	, _mon(Q_NULLPTR), _tues(Q_NULLPTR), _wednes(Q_NULLPTR), _thurs(Q_NULLPTR), _fri(Q_NULLPTR), _sat(Q_NULLPTR), _sun(Q_NULLPTR), _lastselect_index(LastClieckType::NONE)
	, _firstmon(Q_NULLPTR), _fifteenmon(Q_NULLPTR), _thirtymon(Q_NULLPTR), _firstrun_week(true), _firstrun_month(true), _pcoff(false)
	, _weektype(ReservWeekType::MON), _monthtype(ReservMonthType::FIRST), m_notuse(false)

{

	setFixedSize(465, 266);

	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/gbdate.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	auto label = new CustomLabel(14, "#FFFFFF", true, 0, 0, this);
	label->setText("日時設定");
	label->setGeometry(15, 15, 465, 266);
	label->show();


	QString ss = ssdefines::imageToggleButton.arg(QString(":/smartopti/resource/preview/smartopti/datebtn.png"))
		.arg(":/smartopti/resource/preview/smartopti/datebtn.png")
		.arg(":/smartopti/resource/preview/smartopti/datebtn.png")
		.arg(":/smartopti/resource/preview/smartopti/datebtn.png")
		.arg(":/smartopti/resource/preview/smartopti/datebtn_click.png");


	{//week
		int posx = 130;
		_mon = new Button(this);
		{
			_mon->setText("月");
			_mon->setCheckable(true);
			_mon->setChecked(true);
			_mon->setStyleSheet(ss);
			_mon->setGeometry(posx, 50, 40, 40);
			_mon->hide();
			connect(_mon, &Button::clicked, [=]() {

				if (m_notuse)
					return;

				if (_mon->isChecked()) {
					_lastselect_index = LastClieckType::MON;
					if (_sun->isChecked())
						_sun->setChecked(false);

					if (_sat->isChecked())
						_sat->setChecked(false);

					if (_fri->isChecked())
						_fri->setChecked(false);

					if (_thurs->isChecked())
						_thurs->setChecked(false);

					if (_wednes->isChecked())
						_wednes->setChecked(false);

					if (_tues->isChecked())
						_tues->setChecked(false);

					_summary_typelabel->setText("毎週(月)");
					_weektype = ReservWeekType::MON;
				}

				});
		}

		_tues = new Button(this);
		{
			_tues->setText("火");
			_tues->setCheckable(true);
			_tues->setStyleSheet(ss);
			_tues->setGeometry(posx += 45, 50, 40, 40);
			_tues->hide();
			connect(_tues, &Button::clicked, [=]() {

				if (_tues->isChecked()) {
					_lastselect_index = LastClieckType::TUES;
					if (_sun->isChecked())
						_sun->setChecked(false);

					if (_sat->isChecked())
						_sat->setChecked(false);

					if (_fri->isChecked())
						_fri->setChecked(false);

					if (_thurs->isChecked())
						_thurs->setChecked(false);

					if (_wednes->isChecked())
						_wednes->setChecked(false);

					if (_mon->isChecked())
						_mon->setChecked(false);

					_summary_typelabel->setText("毎週(火)");
					_weektype = ReservWeekType::TUES;
				}

				});
		}

		_wednes = new Button(this);
		{
			_wednes->setText("水");
			_wednes->setCheckable(true);
			_wednes->setStyleSheet(ss);
			_wednes->setGeometry(posx += 45, 50, 40, 40);
			_wednes->hide();
			connect(_wednes, &Button::clicked, [=]() {

				if (_wednes->isChecked()) {
					_lastselect_index = LastClieckType::WEDNES;
					if (_sun->isChecked())
						_sun->setChecked(false);

					if (_sat->isChecked())
						_sat->setChecked(false);

					if (_fri->isChecked())
						_fri->setChecked(false);

					if (_thurs->isChecked())
						_thurs->setChecked(false);

					if (_tues->isChecked())
						_tues->setChecked(false);

					if (_mon->isChecked())
						_mon->setChecked(false);

					_summary_typelabel->setText("毎週(水)");
					_weektype = ReservWeekType::WEDNES;
				}

				});
		}

		_thurs = new Button(this);
		{
			_thurs->setText("木");
			_thurs->setCheckable(true);
			_thurs->setStyleSheet(ss);
			_thurs->setGeometry(posx += 45, 50, 40, 40);
			_thurs->hide();
			connect(_thurs, &Button::clicked, [=]() {

				if (_thurs->isChecked()) {
					_lastselect_index = LastClieckType::THURS;
					if (_sun->isChecked())
						_sun->setChecked(false);

					if (_sat->isChecked())
						_sat->setChecked(false);

					if (_fri->isChecked())
						_fri->setChecked(false);

					if (_wednes->isChecked())
						_wednes->setChecked(false);

					if (_tues->isChecked())
						_tues->setChecked(false);

					if (_mon->isChecked())
						_mon->setChecked(false);

					_summary_typelabel->setText("毎週(木)");
					_weektype = ReservWeekType::THURS;
				}

				});
		}

		_fri = new Button(this);
		{
			_fri->setText("金");
			_fri->setCheckable(true);
			_fri->setStyleSheet(ss);
			_fri->setGeometry(posx += 45, 50, 40, 40);
			_fri->hide();
			connect(_fri, &Button::clicked, [=]() {

				if (_fri->isChecked()) {
					_lastselect_index = LastClieckType::FRI;
					if (_sun->isChecked())
						_sun->setChecked(false);

					if (_sat->isChecked())
						_sat->setChecked(false);

					if (_thurs->isChecked())
						_thurs->setChecked(false);

					if (_wednes->isChecked())
						_wednes->setChecked(false);

					if (_tues->isChecked())
						_tues->setChecked(false);

					if (_mon->isChecked())
						_mon->setChecked(false);

					_summary_typelabel->setText("毎週(金)");

					_weektype = ReservWeekType::FRI;
				}

				});
		}

		_sat = new Button(this);
		{
			_sat->setText("土");
			_sat->setCheckable(true);
			_sat->setStyleSheet(ss);
			_sat->setGeometry(posx += 45, 50, 40, 40);
			_sat->hide();
			connect(_sat, &Button::clicked, [=]() {

				if (_sat->isChecked()) {
					_lastselect_index = LastClieckType::SAT;
					if (_sun->isChecked())
						_sun->setChecked(false);

					if (_fri->isChecked())
						_fri->setChecked(false);

					if (_thurs->isChecked())
						_thurs->setChecked(false);

					if (_wednes->isChecked())
						_wednes->setChecked(false);

					if (_tues->isChecked())
						_tues->setChecked(false);

					if (_mon->isChecked())
						_mon->setChecked(false);

					_summary_typelabel->setText("毎週(土)");

					_weektype = ReservWeekType::SAT;
				}

				});
		}

		_sun = new Button(this);
		{
			_sun->setText("日");
			_sun->setCheckable(true);
			_sun->setStyleSheet(ss);
			_sun->setGeometry(posx += 45, 50, 40, 40);
			_sun->hide();
			connect(_sun, &Button::clicked, [=]() {

				if (_sun->isChecked()) {
					_lastselect_index = LastClieckType::SUN;
					if (_sat->isChecked())
						_sat->setChecked(false);

					if (_fri->isChecked())
						_fri->setChecked(false);

					if (_thurs->isChecked())
						_thurs->setChecked(false);

					if (_wednes->isChecked())
						_wednes->setChecked(false);

					if (_tues->isChecked())
						_tues->setChecked(false);

					if (_mon->isChecked())
						_mon->setChecked(false);

					_summary_typelabel->setText("毎週(日)");

					_weektype = ReservWeekType::SUN;
				}

				});
		}
	}

	{ //month
		int posx = 130;
		_firstmon = new Button(this);
		{
			_firstmon->setText("毎月1日");
			_firstmon->setCheckable(true);
			_firstmon->setChecked(true);
			_firstmon->setStyleSheet(ss);
			_firstmon->setGeometry(posx, 50, 100, 40);
			_firstmon->hide();
			connect(_firstmon, &Button::clicked, [=]() {

				_lastselect_index = LastClieckType::FIRST;

				if (_firstmon->isChecked()) {
					if (_thirtymon->isChecked())
						_thirtymon->setChecked(false);

					if (_fifteenmon->isChecked())
						_fifteenmon->setChecked(false);

					_summary_typelabel->setText("毎月1日");
					_monthtype = ReservMonthType::FIRST;
				}

				});
		}

		_fifteenmon = new Button(this);
		{
			_fifteenmon->setText("毎月15日");
			_fifteenmon->setCheckable(true);
			_fifteenmon->setStyleSheet(ss);
			_fifteenmon->setGeometry(posx += 105, 50, 100, 40);
			_fifteenmon->hide();
			connect(_fifteenmon, &Button::clicked, [=]() {

				_lastselect_index = LastClieckType::FIFTEEN;

				if (_fifteenmon->isChecked()) {
					if (_thirtymon->isChecked())
						_thirtymon->setChecked(false);

					if (_firstmon->isChecked())
						_firstmon->setChecked(false);

					_summary_typelabel->setText("毎月15日");
					_monthtype = ReservMonthType::FIFTEEN;
				}

				});
		}

		_thirtymon = new Button(this);
		{
			_thirtymon->setText("毎月最終日");
			_thirtymon->setCheckable(true);
			_thirtymon->setStyleSheet(ss);
			_thirtymon->setGeometry(posx += 105, 50, 100, 40);
			_thirtymon->hide();
			connect(_thirtymon, &Button::clicked, [=]() {

				_lastselect_index = LastClieckType::THIRTY;

				if (_thirtymon->isChecked()) {
					if (_fifteenmon->isChecked())
						_fifteenmon->setChecked(false);

					if (_firstmon->isChecked())
						_firstmon->setChecked(false);

					_summary_typelabel->setText("毎月最終日");
					_monthtype = ReservMonthType::THIRTY;
				}

				});

		}
	}

	_day = new Button(this);
	{
		_day->setText("毎日");
		_day->setCheckable(true);
		_day->setStyleSheet(ss);
		_day->setGeometry(15, 70, 101, 40);
		_day->setChecked(true);

		connect(_day, &Button::clicked, [=]() {
			enableWeekButton(false);
			enableMonthButton(false);
			
			if (_day->isChecked()) {
				_lastselect_index = LastClieckType::NONE;

				if (_week->isChecked()) {
					_week->setEnabled(true);
					_week->setChecked(false);
				}
					

				if (_month->isChecked()) {
					_month->setEnabled(true);
					_month->setChecked(false);
				}
					
				
				_day->setEnabled(false);

				_summary_typelabel->setText("毎日");
			}

			});
	}
	_day->show();

	_week = new Button(this);
	{
		_week->setText("毎週");
		_week->setCheckable(true);
		_week->setStyleSheet(ss);
		_week->setGeometry(15, 120, 101, 40);
		connect(_week, &Button::clicked, [=]() {
			enableMonthButton(false);
			enableWeekButton(true);
			

			if (_week->isChecked()) {

				if (_day->isChecked()) {
					_day->setEnabled(true);
					_day->setChecked(false);
				}
					

				if (_month->isChecked()) {
					_month->setEnabled(true);
					_month->setChecked(false);
				}
					
				_week->setEnabled(false);

				updateSummaryTypeLabel();
			}


			});
	}
	_week->show();

	

	_month = new Button(this);
	{
		_month->setText("毎月");
		_month->setCheckable(true);
		_month->setStyleSheet(ss);
		_month->setGeometry(15, 170, 101, 40);
		connect(_month, &Button::clicked, [=]() {
			//dropdownbg->show();

			enableWeekButton(false);
			enableMonthButton(true);

			if (_month->isChecked()) {
				if (_day->isChecked()) {
					_day->setEnabled(true);
					_day->setChecked(false);
				}
					

				if (_week->isChecked()) {
					_week->setEnabled(true);
					_week->setChecked(false);
				}

				_month->setEnabled(false);
			}

			if(_firstmon->isChecked())
				_summary_typelabel->setText("毎月1日");
			else if(_fifteenmon->isChecked())
				_summary_typelabel->setText("毎月15日");
			else if (_thirtymon->isChecked())
				_summary_typelabel->setText("毎月最終日");

			});
	}
	_month->show();

	int addposY = 16;
	_combowidget = new ComboWidget(this);
	_combowidget->setGeometry(135, 80 + addposY, 124, 98);
	_combowidget->show();

	_summary = new QLabel(this);
	_summary->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/summary.png"));
	_summary->setGeometry(282, 80 + addposY , 162, 98);
	_summary->show();
	
	_summary_typelabel = new CustomLabel(16, "#ffffff", true, 0, 0, this);
	_summary_typelabel->setText("毎日");
	_summary_typelabel->setGeometry(282, 100 + addposY, 162, 20);
	_summary_typelabel->setAlignment(Qt::AlignCenter);
	_summary_typelabel->show();

	_summary_label = new CustomLabel(16, "#ffffff", true, 0, 0, this);
	_summary_label->setText("12 : 00");
	_summary_label->setGeometry(282, 124 + addposY, 162, 20);
	_summary_label->setAlignment(Qt::AlignCenter);
	_summary_label->show();

	ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{color:#23262A; font-weight:bold;}");
	_insertbtn = new Button(this);
	_insertbtn->setStyleSheet(ss);
	_insertbtn->setText("設定");
	_insertbtn->setGeometry(282, 180 + addposY, 162, 40);
	_insertbtn->show();
	connect(_insertbtn, &Button::clicked, [=]() {

		if (m_notuse) {
			emit buttonNotUseAlert();
			return;
		}
		
		QString text = QString("%1 %2").arg(_summary_typelabel->text()).arg(_summary_label->text());
		
		if (_pcoff) {
			emit insertPcOff(getReserveData());
		}
		else {
			emit insertResve(text, getReserveData());
		}

		});


	QObject::connect(_combowidget, &ComboWidget::sendSetTime, [=](QString settime) {
		_summary_label->setText(settime);
		});


	QObject::connect(this, SIGNAL(updatetime(int, int)), _combowidget, SLOT(setComboTime(int, int)));
	
}

SetScheduleContainer::~SetScheduleContainer()
{}


void SetScheduleContainer::enableWeekButton(bool enable)
{
	if (enable) {
		_mon->show();
		_tues->show();
		_wednes->show();
		_thurs->show();
		_fri->show();
		_sat->show();
		_sun->show();
	
	}
	else {
		_mon->hide();
		_tues->hide();
		_wednes->hide();
		_thurs->hide();
		_fri->hide();
		_sat->hide();
		_sun->hide();
	}
	
}

void SetScheduleContainer::enableMonthButton(bool enable)
{
	if (enable) {
		_firstmon->show();
		_fifteenmon->show();
		_thirtymon->show();
	}
	else {
		_firstmon->hide();
		_fifteenmon->hide();
		_thirtymon->hide();
	}
}


void SetScheduleContainer::updateSummaryTypeLabel()
{
	if (_mon->isChecked())
		_summary_typelabel->setText("毎週(月)");
	else if (_tues->isChecked())
		_summary_typelabel->setText("毎週(火)");
	else if (_wednes->isChecked())
		_summary_typelabel->setText("毎週(水)");
	else if (_thurs->isChecked())
		_summary_typelabel->setText("毎週(木)");
	else if (_fri->isChecked())
		_summary_typelabel->setText("毎週(金)");
	else if (_sat->isChecked())
		_summary_typelabel->setText("毎週(土)");
	else if (_sun->isChecked())
		_summary_typelabel->setText("毎週(日)");
}

void SetScheduleContainer::updateWeekTypes(ReservWeekType type)
{
	switch (type)
	{
	case ReservWeekType::MON:
		//_mon->click();
		//_summary_typelabel->setText("毎週(月)");
		break;
	case ReservWeekType::TUES:
		_tues->click();
		_summary_typelabel->setText("毎週(火)");
		break;
	case ReservWeekType::WEDNES:
		_wednes->click();
		_summary_typelabel->setText("毎週(水)");
		break;
	case ReservWeekType::THURS:
		_thurs->click();
		_summary_typelabel->setText("毎週(木)");
		break;
	case ReservWeekType::FRI:
		_fri->click();
		_summary_typelabel->setText("毎週(金)");
		break;
	case ReservWeekType::SAT:
		_sat->click();
		_summary_typelabel->setText("毎週(土)");
		break;
	case ReservWeekType::SUN:
		_sun->click();
		_summary_typelabel->setText("毎週(日)");
		break;
	case ReservWeekType::NONE:
		_mon->click();
		_summary_typelabel->setText("毎週(月)");
		break;
	default:
		_mon->click();
		_summary_typelabel->setText("毎週(月)");
		break;
	}

}

void SetScheduleContainer::updateMonthTypes(ReservMonthType type)
{
	switch (type)
	{
	case ReservMonthType::FIRST:
		//_firstmon->click();
		//_summary_typelabel->setText("毎月1日");
		break;
	case ReservMonthType::FIFTEEN:
		_fifteenmon->click();
		//_fifteenmon->setChecked(true);
		_summary_typelabel->setText("毎月15日");
		break;
	case ReservMonthType::THIRTY:
		_thirtymon->click();
		//_thirtymon->setChecked(true);
		_summary_typelabel->setText("毎月最終日");
		break;
	case ReservMonthType::NONE:
		//_firstmon->click();
		//_firstmon->setChecked(true);
		//_summary_typelabel->setText("毎月1日");
		break;
	default:
		//_firstmon->click();
		//_firstmon->setChecked(true);
		//_summary_typelabel->setText("毎月1日");
		break;
	}
}

QString SetScheduleContainer::getStringbyWeekType(ReservWeekType type)
{
	QString typestr;
	switch (type)
	{
	case ReservWeekType::MON:
		typestr = "毎週(月)";
		break;
	case ReservWeekType::TUES:
		typestr = "毎週(火)";
		break;
	case ReservWeekType::WEDNES:
		typestr = "毎週(水)";
		break;
	case ReservWeekType::THURS:
		typestr = "毎週(木)";
		break;
	case ReservWeekType::FRI:
		typestr = "毎週(金)";
		break;
	case ReservWeekType::SAT:
		typestr = "毎週(土)";
		break;
	case ReservWeekType::SUN:
		typestr = "毎週(日)";
		break;
	case ReservWeekType::NONE:
		typestr = "";
		break;
	default:
		typestr = "";
		break;
	}

	return typestr;
}

QString SetScheduleContainer::getStringbyMonthType(ReservMonthType type)
{
	QString typestr;
	switch (type)
	{
	case ReservMonthType::FIRST:
		typestr = "毎月1日";
		break;
	case ReservMonthType::FIFTEEN:
		typestr = "毎月15日";
		break;
	case ReservMonthType::THIRTY:
		typestr = "毎月最終日";
		break;
	case ReservMonthType::NONE:
		typestr = "";
		break;
	default:
		typestr = "";
		break;
	}

	return typestr;
}

//사용 안함일 경우 비활성화
void SetScheduleContainer::notuseUpdate()
{
	bool enable = true;
	if (m_notuse)  //사용안함
		enable = false;
	else 
		enable = true;

	_day->setEnabled(enable);
	_week->setEnabled(enable);
	_month->setEnabled(enable);

	_mon->setEnabled(enable);
	_tues->setEnabled(enable);
	_wednes->setEnabled(enable);
	_thurs->setEnabled(enable);
	_fri->setEnabled(enable);
	_sat->setEnabled(enable);
	_sun->setEnabled(enable);

	_firstmon->setEnabled(enable);
	_fifteenmon->setEnabled(enable);
	_thirtymon->setEnabled(enable);

	_combowidget->setEnabled(enable);

	if (enable == false) {
		if (_day->isChecked())
			_day->setChecked(false);
		else if (_week->isChecked()) {
			_week->setChecked(false);

			if (_mon->isChecked())
				_mon->setChecked(false);
			else if (_tues->isChecked())
				_tues->setChecked(false);
			else if (_wednes->isChecked())
				_wednes->setChecked(false);
			else if (_thurs->isChecked())
				_thurs->setChecked(false);
			else if (_fri->isChecked())
				_fri->setChecked(false);
			else if (_sat->isChecked())
				_sat->setChecked(false);
			else if (_sun->isChecked())
				_sun->setChecked(false);

		}
		else if (_month->isChecked()) {
			_month->setChecked(false);

			if (_firstmon->isChecked())
				_firstmon->setChecked(false);
			else if (_fifteenmon->isChecked())
				_fifteenmon->setChecked(false);
			else if (_thirtymon->isChecked())
				_thirtymon->setChecked(false);
		}
			

		

		

	}

}

void SetScheduleContainer::useUiUpdate()
{
	switch (_lastselect_index)
	{
	case LastClieckType::NONE:
		_day->setChecked(true);
		break;
	case LastClieckType::MON:
		_week->setChecked(true);
		_mon->setChecked(true);
		break;
	case LastClieckType::TUES:
		_week->setChecked(true);
		_tues->setChecked(true);
		break;
	case LastClieckType::WEDNES:
		_week->setChecked(true);
		_wednes->setChecked(true);
		break;
	case LastClieckType::THURS:
		_week->setChecked(true);
		_thurs->setChecked(true);
		break;
	case LastClieckType::FRI:
		_week->setChecked(true);
		_fri->setChecked(true);
		break;
	case LastClieckType::SAT:
		_week->setChecked(true);
		_sat->setChecked(true);
		break;
	case LastClieckType::SUN:
		_week->setChecked(true);
		_sun->setChecked(true);
		break;
	case LastClieckType::FIRST:
		_month->setChecked(true);
		_firstmon->setChecked(true);
		break;
	case LastClieckType::FIFTEEN:
		_month->setChecked(true);
		_fifteenmon->setChecked(true);
		break;
	case LastClieckType::THIRTY:
		_month->setChecked(true);
		_thirtymon->setChecked(true);
		break;
	default:
		_day->setChecked(true);
		break;
	}

	/*if (!_mon->isHidden()) {
		_week->setChecked(true);
		_mon->setChecked(true);
	}
	else if (_firstmon->isHidden()) {
		_month->setChecked(true);
		_firstmon->setChecked(true);
	}
	else
		_day->setChecked(true);*/
}

ReserveData SetScheduleContainer::getReserveData()
{
	ReserveData data;

	if (_day->isChecked())
		data.type = ReservType::DAY;
	else if (_week->isChecked()) {
		data.type = ReservType::WEEK;
		data.weektype = _weektype;
	}
	else if (_month->isChecked()) {
		data.type = ReservType::MONTH;
		data.monthtype = _monthtype;
	}
	else return data;

	data.hour = _combowidget->getHour();
	data.min = _combowidget->getMin();
	data.use = true;
	return data;
}

void SetScheduleContainer::updateReserveData(ReserveData data)
{
	//updateSummaryTypeLabel()
	//_summary_typelabel
	if (data.type == ReservType::DAY) {
		//_day->click();
	}
	else if (data.type == ReservType::WEEK) {
		_week->click();
		_weektype = data.weektype;
		updateWeekTypes(data.weektype);
	}
	else if (data.type == ReservType::MONTH) {
		_month->click();
		_monthtype = data.monthtype;
		updateMonthTypes(data.monthtype);
	}
	else return;

	_summary_label->setText(QString("%1 : %2").arg(QStringLiteral("%1").arg(data.hour, 2, 10, QLatin1Char('0'))).arg(QStringLiteral("%1").arg(data.min, 2, 10, QLatin1Char('0'))));
	updatetime(data.hour, data.min);

	if (data.use == false) {
		setNotUse(true);
	}
	else {
		setNotUse(false);
	}

	notuseUpdate();
}

