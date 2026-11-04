#include "optimizeresulwidget.h"

#include <QLabel>

#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"

#include "../../common/defines/ssdefins.h"

OptimizeResultWidget::OptimizeResultWidget(const QString imagpath, const QString title, QWidget *parent)
	: QWidget(parent), _count(Q_NULLPTR), _size(Q_NULLPTR)
{
	//ui.setupUi(this);
	this->setCursor(Qt::PointingHandCursor);

	int beforeposx = 0, beforeposy = 0;
	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/preopti/resource/preview/opti/optibox.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	auto arrow = new QLabel(this);
	arrow->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/underarrow.png"));
	arrow->setGeometry(175, 26, 16, 17);
	arrow->show();

	this->setFixedSize(221,202);
	auto title_img = new QLabel(this);
	title_img->setStyleSheet(ssdefines::imageWidget.arg(imagpath));
	title_img->setGeometry(20,20, 30,30);
	title_img->show();

	auto title_text = new CustomLabel(24, "#FFFFFF", true, 0, 0, this);
	title_text->setText(title);
	title_text->setGeometry(50, 22, 200, 30);
	title_text->show();

	_count = new CustomLabel(15, "#A9ABAD", true, 0, 0, this);
	_count->setGeometry(20, 110, 200, 20);
	_count->hide();

	_size = new CustomLabel(15, "#A9ABAD", true, 0, 0, this);
	_size->setGeometry(20, 80, 200, 20);
	_size->show();
}

OptimizeResultWidget::~OptimizeResultWidget()
{}


void OptimizeResultWidget::setOptiText(QString count, QString size)
{
	_count->setText(count);
	
	_count->show();

	if (size != Q_NULLPTR) {
		_size->setText(size);
		
		_size->show();
	}
}

void OptimizeResultWidget::mousePressEvent(QMouseEvent* event) 
{
	if (event->button() == Qt::LeftButton) {
		emit sendClicked();
	}
		
	
	QWidget::mousePressEvent(event);
}
