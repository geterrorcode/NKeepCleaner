#include "topinfowidget.h"

#include <QImage>

TopInfoWidget::TopInfoWidget(const QString bgimg, const QString detailimg, QWidget *parent)
	: QWidget(parent)
{
	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(bgimg));
		baseLayout->addWidget(backgroundFrame);
	}

	if (!detailimg.isEmpty()) {

		auto detail = new QLabel(this);
		auto imageinfo = new QImage(detailimg);

		detail->setStyleSheet(ssdefines::imageWidget.arg(detailimg));
		detail->setFixedSize(imageinfo->size());
		baseLayout->addWidget(detail);
		baseLayout->setAlignment(detail, Qt::AlignCenter);
	}
	
}

TopInfoWidget::~TopInfoWidget()
{}
