#include "backbuttonwidget.h"

#include "../../QuickGrip_tmp/QuickGrip_tmp.h"

#include <view/Component/button.h>
#include <common/defines/ssdefins.h>


BackButtonWidget::BackButtonWidget(QWidget *parent)
	: QWidget(parent)
{
	
	this->setStyleSheet("QWidget {background-color: #2E3135; border: 1px solid #cecece; border-radius: 3px;}"
	"QToolTip { color: #DDDDDD; background-color: #444444; border: 1px solid white; }");
	
	auto arrow = new QLabel(this);
	arrow->setStyleSheet(ssdefines::imageWidget.arg(":/common/resource/common/btn_back.png"));
	arrow->setGeometry(0, 0, 40, 40);
	arrow->show();

	
	setMouseTracking(true);
	//this->setCursor(Qt::PointingHandCursor);
}

BackButtonWidget::~BackButtonWidget()
{}


void BackButtonWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		emit backButtonClicked();

	QWidget::mousePressEvent(event);
}

bool BackButtonWidget::event(QEvent* event) 
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

		// Get current mouse position
		QPoint mousePos = QCursor::pos();

		// Adjust position for tooltip
		QPoint toolTipPos = QPoint(mousePos.x() + 12, mousePos.y() - 20);

		QToolTip::showText(toolTipPos, "ホームに戻る", this);
	}

	return QWidget::event(event);
}