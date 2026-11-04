#include "settimewidget.h"

#include "view/Component/button.h"


#include "../Container/topinfowidget.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

SetTimeWidget::SetTimeWidget(QWidget *parent)
	: QWidget(parent)
{

	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/pcoff/resource/preview/pcoff/gbbody.png"));
		baseLayout->addWidget(backgroundFrame);
	}



}

SetTimeWidget::~SetTimeWidget()
{}
