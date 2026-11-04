#include "traypopup.h"

#include <QTimer>

#include <common/ui/movetrayarea.h>

#include "view/Component/hlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"

#include <common/tool/utils.h>
#include "../../ui.h"
#include "../../common/defines/ssdefins.h"


TrayPopup::TrayPopup(const QString& body, QWidget *parent)
	: QWidget(parent), _bodyLabel(Q_NULLPTR), _traytimer(Q_NULLPTR)
{ 

	int sizeX = 360;
	int sizeY = 205;

	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

	setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setFixedSize(360, 205);
	setFocusPolicy(Qt::NoFocus);

	moveTrayArea(this);

	auto baseLayout = new ContainerLayout(this);

	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet("background-color: #2E3135; border:1px solid #cbcac9; border-radius:10px;");
		baseLayout->addWidget(backgroundFrame);
	}

	auto title = new QLabel(this);
	{
		title->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/title.png"));
		title->setGeometry(3, 3, 217, 52);
		title->show();
	}

	auto exit = new Button(this);
	{
		exit->setCursor(Qt::PointingHandCursor);
		exit->setStyleSheet(ssdefines::imageButton.arg(":main/resource/main/close.png",
			":main/resource/main/close_over.png",
			":main/resource/main/close_press.png",
			":main/resource/main/close.png"));

		//connect(exit, &Button::clicked, this, &QWidget::close);
		connect(exit, &Button::clicked, this, &QWidget::hide);

		exit->setGeometry(sizeX - 40, 10, 30, 30);
		exit->show();
	}

	_bodyLabel = new QLabel(this);
	_bodyLabel->setAlignment(Qt::AlignCenter);
	_bodyLabel->setWordWrap(true);
	_bodyLabel->setGeometry(0, 40, sizeX, sizeY - 40);
	_bodyLabel->setStyleSheet(ssdefines::label.arg("18px", "#FFFFFF", "none"));
	_bodyLabel->setText(utils::toRichText(body, 18));
	_bodyLabel->show();

	//QTimer::singleShot(10000, this, &QWidget::close);

	_traytimer = new QTimer(this);
	connect(_traytimer, &QTimer::timeout, [=]() {
		this->hide();
		});

	connect(rcv(this), &Receiver::clicked, [=]() {
		window()->show();
		hide();

		});

	hide();
}

TrayPopup::~TrayPopup()
{
	if (_traytimer) {
		if (_traytimer.data()->isActive())
			_traytimer.data()->stop();

		_traytimer->deleteLater();
	}
	


}


void TrayPopup::setBody(QString body)
{
	_bodyLabel->setText(utils::toRichText(body, 18));
}

void TrayPopup::startTimer(int msec)
{
	if (_traytimer.data()->isActive())
		_traytimer.data()->stop();

	_traytimer->setInterval(msec);
	_traytimer.data()->start();

}

void TrayPopup::stopTimer()
{
	if (_traytimer.data()->isActive())
		_traytimer.data()->stop();
}