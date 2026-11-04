#include "mainbottomcontainer.h"

#include <QLabel>


#include "../../common/defines/ssdefins.h"


MainBottomContainer::MainBottomContainer(QString imgpath, QString name, QString data, QString tooltip, QWidget *parent)
	: QPushButton(parent), _img(Q_NULLPTR), _bottom_data(Q_NULLPTR), _imgpath(imgpath), _data(data), _tooltip(tooltip)
{
	setFixedSize(139, 204);
	setFocusPolicy(Qt::NoFocus);
	this->setCursor(Qt::PointingHandCursor);
	this->setStyleSheet("QWidget {background: transparent; font: normal bold 14px;}"
		"QToolTip { color: #DDDDDD; background-color: #444444; border: 1px solid white; }");

	_img  = new QWidget(this);
	_img->setStyleSheet(ssdefines::imageWidget.arg(imgpath));
	_img->setGeometry(0, 0, 139, 140);
	_img->show();

	auto bottom_name = new CustomLabel(18, "#DDDDDD", true, 0, 0, this);
	bottom_name->setAlignment(Qt::AlignCenter);
	bottom_name->setText(name);
	bottom_name->setGeometry(0, 150, 139, 26);
	bottom_name->show();

	_bottom_data = new CustomLabel(14, "#AAAAAA", true, 0, 40, this);
	_bottom_data->setAlignment(Qt::AlignCenter);
	_bottom_data->setText(_data);
	_bottom_data->setGeometry(0, 170, 139, 40);
	_bottom_data->show();
}

MainBottomContainer::~MainBottomContainer()
{}


void MainBottomContainer::setUpdate(QString imgpath, QString data)
{
	_imgpath = imgpath;
	_data = data;
	_img->setStyleSheet(ssdefines::imageWidget.arg(_imgpath));
	_bottom_data->setText(_data);
}

bool MainBottomContainer::event(QEvent* event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

		// Get current mouse position
		QPoint mousePos = QCursor::pos();

		// Adjust position for tooltip
		QPoint toolTipPos = QPoint(mousePos.x() + 12, mousePos.y() - 20);

		QToolTip::showText(toolTipPos, _tooltip, this);
	}

	return QWidget::event(event);
}