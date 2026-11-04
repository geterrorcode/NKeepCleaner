#include "autopcoffview.h"

#include <QComboBox>
#include <QCheckBox>

#include "view/Component/vlayout.h"
#include "view/Component/button.h"
#include "view/common/alertpopup.h"
#include "view/common/traypopup.h"

#include <controller/auto_pcoff/autopcoffcontroller.h>


#include "../Container/topinfowidget.h"
#include "../smart_optimize/setschedulecontainer.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

AutoPcOffView::AutoPcOffView(QWidget *parent)
	: QWidget(parent)  , _usecheck(Q_NULLPTR), _offcheck(Q_NULLPTR)
{

	int marginsize_x = 218;
	int marginsize_y = 50;

	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	auto topbase = new QLabel(this);
	topbase->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/top.png"));
	topbase->setGeometry(258 - marginsize_x, 76 - marginsize_y, 710, 74);
	topbase->show();

	auto toplabel = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
	toplabel->setGeometry(258 - marginsize_x, 98 - marginsize_y, 750, 26);
	toplabel->setAlignment(Qt::AlignCenter);
	toplabel->setText(QString("<font color=#FDBB30>%1</font>に自動でPCがシャットダウンされます").arg("設定した時間"));
	toplabel->show();

	auto title = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
	title->setText("PC自動シャットダウン設定");
	title->setGeometry(318 - marginsize_x, 243 - marginsize_y, 180, 20);
	title->show();

	auto summarybg = new QLabel(this);
	summarybg->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/gbonoff.png"));
	summarybg->setGeometry(558 - marginsize_x, 248 - marginsize_y, 200, 80);
	summarybg->show();
	
	auto listtext = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
	listtext->setText("設定済みのリスト");
	listtext->setGeometry(566 - marginsize_x, 256 - marginsize_y, 180, 24);
	listtext->show();

	auto summarylabel = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	summarylabel->setGeometry(576 - marginsize_x, 290 - marginsize_y, 230, 40);
	summarylabel->show();

	auto schedule = new SetScheduleContainer(this);
	schedule->setGeometry(302 - marginsize_x, 340 - marginsize_y, 710, 386);
	schedule->setPcAutoOffType(true);
	schedule->show();

	QObject::connect(schedule, &SetScheduleContainer::insertPcOff, [=](ReserveData data) {

		ReserveData udpatedata;
		bool use = false;
		if (_usecheck->isChecked() == true)
			use = true;

		udpatedata.use = use;
		udpatedata.hour = data.hour;
		udpatedata.min = data.min;
		udpatedata.type = data.type;
		udpatedata.weektype = data.weektype;
		udpatedata.monthtype = data.monthtype;


		emit updatePcOffData(udpatedata);
		//QPushButton{ font: bold normal 15px; color:#FFFFFF; }
		if (udpatedata.use) {
			summarylabel->setColor("#FFFFFF");
			//summarylabel->colorChanged("#FFFFFF");
		}
		else {
			summarylabel->setColor("#666666");
			//summarylabel->colorChanged("#666666");
		}
			
			//summarylabel->setStyleSheet("QPushButton {color:#666666;} ");


		QString timesummary = QString("%1 : %2").arg(QStringLiteral("%1").arg(data.hour, 2, 10, QLatin1Char('0'))).arg(QStringLiteral("%1").arg(data.min, 2, 10, QLatin1Char('0')));

		QString summary = "";
		if (data.type == ReservType::WEEK)
			summary = QString("• %1 %2").arg(schedule->getStringbyWeekType(data.weektype)).arg(timesummary);
		else if (data.type == ReservType::MONTH)
			summary = QString("• %1 %2").arg(schedule->getStringbyMonthType(data.monthtype)).arg(timesummary);
		else
			summary = QString("• 毎日 %2").arg(timesummary);

		summarylabel->setText(summary);

		});

	
	_offcheck = new QCheckBox(this);
	_offcheck->setStyleSheet(ssdefines::checkboxFixImage);
	_offcheck->setGeometry(322 + 121- marginsize_x, 293 - marginsize_y, 180, 20);
	_offcheck->setCheckable(true);
	_offcheck->setChecked(true); //default
	_offcheck->setText("OFF");
	_offcheck->show();
	connect(_offcheck, &Button::clicked, [=]() {

		if (_offcheck->isChecked()) {

			summarylabel->setColor("#666666");

			schedule->setNotUse(true); //not use on
			schedule->notuseUpdate();

			_offcheck->setChecked(true);
			_offcheck->setEnabled(false);

			_usecheck->setChecked(false);
			_usecheck->setEnabled(true);
			emit PcOffOptionOnOff(false);
		}

		});

	_usecheck = new QCheckBox(this);
	_usecheck->setStyleSheet(ssdefines::checkboxFixImage);
	_usecheck->setGeometry(322 - marginsize_x, 293 - marginsize_y, 100, 20);
	_usecheck->setCheckable(true);
	_usecheck->setText("ON");
	_usecheck->show();
	connect(_usecheck, &Button::clicked, [=]() {

		if (_usecheck->isChecked()) {

			summarylabel->setColor("#FFFFFF");

			schedule->setNotUse(false); //use 
			schedule->useUiUpdate(); //use 
			schedule->notuseUpdate();

			_usecheck->setChecked(true);
			_usecheck->setEnabled(false);

			_offcheck->setChecked(false);
			_offcheck->setEnabled(true);
			emit PcOffOptionOnOff(true);
			
			//schedule->show();
		}

		});

	auto desc = new CustomLabel(15, "#FFFFFF", false, 0, 50, this);
	desc->setText("※PCシャットダウン予約は１つのみ設定可能です");
	desc->setGeometry(258 - marginsize_x, 606 - marginsize_y, 465, 50);
	desc->setWordWrap(true);
	desc->show();


	// PC自動終了設定
	setFixedSize(parent->size());

	/*bool use = AutoPcOffController::inst()->isUse();
	if (use) {
		check->setChecked(true);
		schedule->show();
	}*/
	
	QObject::connect(this, SIGNAL(updatePcOffReserveData(ReserveData)), schedule, SLOT(updateReserveData(ReserveData)));
	//초기 설정값 세팅
	QObject::connect(AutoPcOffController::inst(), &AutoPcOffController::updatePcOffViewData, [=](ReserveData data) {
		if (data.use) {
			_usecheck->setChecked(true);
			_offcheck->setChecked(false);
			summarylabel->setColor("#FFFFFF");
			
		}
		else {
			_offcheck->setChecked(true);
			_usecheck->setChecked(false);
			summarylabel->setColor("#666666");
		}
			
		emit updatePcOffReserveData(data);

		QString timesummary = QString("%1 : %2").arg(QStringLiteral("%1").arg(data.hour, 2, 10, QLatin1Char('0'))).arg(QStringLiteral("%1").arg(data.min, 2, 10, QLatin1Char('0')));

		QString summary = "";
		if (data.type == ReservType::WEEK) 
			summary = QString("• %1 %2").arg(schedule->getStringbyWeekType(data.weektype)).arg(timesummary);
		else if (data.type == ReservType::MONTH) 
			summary = QString("• %1 %2").arg(schedule->getStringbyMonthType(data.monthtype)).arg(timesummary);
		else
			summary = QString("• 毎日 %2").arg(timesummary);
		
		summarylabel->setText(summary);

		});


	QObject::connect(this, SIGNAL(getPcOffReserveData()), AutoPcOffController::inst(), SLOT(receiveData()));
	emit getPcOffReserveData();

	QObject::connect(this, SIGNAL(updatePcOffData(ReserveData)), AutoPcOffController::inst(), SLOT(setPcOffData(ReserveData)));
	QObject::connect(this, SIGNAL(PcOffOptionOnOff(bool)), AutoPcOffController::inst(), SLOT(setPcOffConfigDisable(bool)));
	
	

	connect(AutoPcOffController::inst(), &AutoPcOffController::updateOK, [=]() {
		auto alert = new AlertPopup(tr("PC自動シャットダウン設定が\r\n完了しました"), QObject::tr("ok"), "", false, window());
		alert->show();

		});

	connect(schedule, &SetScheduleContainer::buttonNotUseAlert, [=]() {
		auto alert = new AlertPopup(tr("設定がOFFになっているため設定することができません。\r\n\r\n設定をするためにはONの状態にしてください。"), QObject::tr("確認"), "", true, window());
		alert->show();

		});

}

AutoPcOffView::~AutoPcOffView()
{}
