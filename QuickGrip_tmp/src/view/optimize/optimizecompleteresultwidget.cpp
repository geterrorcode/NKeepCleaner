#include "optimizecompleteresultwidget.h"

#include <QLabel>

#include "view/Component/containerlayout.h"
#include "../../common/defines/ssdefins.h"

OptimizeCompleteResultWidget::OptimizeCompleteResultWidget(const QString imagpath, const QString title, QWidget *parent)
	: QWidget(parent), _success_count(Q_NULLPTR), _success_size(Q_NULLPTR), _fail_count(Q_NULLPTR), _fail_size(Q_NULLPTR),
	_successlabel(Q_NULLPTR), _faillabel(Q_NULLPTR)
{
	this->setCursor(Qt::PointingHandCursor);
	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/preopti/resource/preview/opti/boxbg.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	setFixedSize(233, 216);

	auto title_img = new QLabel(this);
	title_img->setStyleSheet(ssdefines::imageWidget.arg(imagpath));
	title_img->setGeometry(15, 10, 30, 30);
	title_img->show();

	auto arrow = new QLabel(this);
	arrow->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/underarrow.png"));
	arrow->setGeometry(190, 16, 16, 17);
	arrow->show();

	auto title_text = new CustomLabel(24, "#FFFFFF", true, 0, 0, this);
	title_text->setText(title);
	//title_text->setStyleSheet(ssdefines::imageWidget.arg(textimagpath));
	title_text->setGeometry(55, 12, 200, 30);
	title_text->show();


	_successlabel = new CustomLabel(18, "#DDDDDD", true, 0, 0, this);
	_successlabel->setGeometry(9, 55, 200, 20);
	_successlabel->setText("最適化完了した項目");
	_successlabel->show();

	_success_count = new CustomLabel(15, "#A9ABAD", true, 0, 0, this);
	_success_count->setGeometry(21, 80, 200, 20);
	_success_count->hide();
	_success_size = new CustomLabel(15, "#A9ABAD", true, 0, 0, this);
	_success_size->setGeometry(21, 100, 200, 20);
	_success_size->hide();

	_faillabel = new CustomLabel(18, "#DDDDDD", true, 0, 0, this);
	_faillabel->setGeometry(9, 135, 200, 20);
	_faillabel->setText("失敗した項目");
	_faillabel->show();

	_fail_count = new CustomLabel(15, "#A9ABAD", true, 0, 0, this);
	_fail_count->setGeometry(21, 160, 200, 20);
	_fail_count->hide();

	_fail_size = new CustomLabel(15, "#A9ABAD", true, 0, 0, this);
	_fail_size->setGeometry(21, 180, 200, 20);
	_fail_size->hide();

}

OptimizeCompleteResultWidget::~OptimizeCompleteResultWidget()
{}


void OptimizeCompleteResultWidget::setOptiCompleteText(QString success_count, QString success_size, QString fail_count, QString fail_size)
{
	if (success_count != Q_NULLPTR) {
		_success_count->setText(success_count);
		_success_count->show();
	}
	else { //레지스트리 비활성화 인경우
		_successlabel->hide();
		_faillabel->hide();
	}


	if (success_size != Q_NULLPTR) {
		_success_size->setText(success_size);
		_success_size->show();
	}
	

	if (fail_count != Q_NULLPTR) {
		_fail_count->setText(fail_count);
		_fail_count->show();
	}


	if (fail_size != Q_NULLPTR) {
		_fail_size->setText(fail_size);
		_fail_size->show();
	}

	//_success_count = success_list[0];

}


void OptimizeCompleteResultWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		emit sendCompleteResultClicked();

	QWidget::mousePressEvent(event);
}
