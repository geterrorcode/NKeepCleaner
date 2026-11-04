#include "pconoffcontainer.h"

#include "view/Component/hlayout.h"

#include "view/Component/customlabel.h"
#include "view/common/alertpopup.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

PcOnOffContainer::PcOnOffContainer(bool ispcon, bool ispcoff, QWidget* parent)
	: QWidget(parent), _pcon(Q_NULLPTR)//, _pcoff(Q_NULLPTR)
{

	//setContentsMargins(6, 36, 0, 6);
	//setSpacing(6);
	setFixedSize(465, 150);

	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/gbonoff.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	auto label = new CustomLabel(14, "#FFFFFF", true, 0, 0, this);
	label->setText("PC最適化タイミング");
	label->setGeometry(15, 15, 200, 20);
	label->show();

	//QString ss_on = ssdefines::imageToggleButton.arg(QString(":/smartopti/resource/preview/smartopti/pcon.png"))
	//	.arg(":/smartopti/resource/preview/smartopti/pcon.png")
	//	.arg(":/smartopti/resource/preview/smartopti/pcon.png")
	//	.arg(":/smartopti/resource/preview/smartopti/pcon.png")
	//	.arg(":/smartopti/resource/preview/smartopti/pcon_click.png");


	QString ss_on = ssdefines::imageToggleButton.arg(QString(":/smartoptimize/resource/smartoptimize/pconoff_btn_disable.png"))
	.arg(":/smartoptimize/resource/smartoptimize/pconoff_btn_disable.png")
	.arg(":/smartoptimize/resource/smartoptimize/pconoff_btn_disable.png")
	.arg(":/smartoptimize/resource/smartoptimize/pconoff_btn_disable.png")
	.arg(":/smartoptimize/resource/smartoptimize/pconoff_btn_check_press.png");
	_pcon = new Button(this);
	{
		_pcon->setText("N-Keepcleaner起動時最適化");
		_pcon->setCheckable(true);
		_pcon->setStyleSheet(ss_on);
		_pcon->setGeometry(100, 55, 253,50);
		
		if (ispcon) {
			_pcon->setChecked(true);
			_pcon->setEnabled(false);
		}

		connect(_pcon, &Button::clicked, [=]() {

			if (_pcon->isChecked()) {
				_pcon->setEnabled(false);
				/*if (_pcoff->isChecked()) {
					_pcoff->setChecked(false);
					_pcoff->setEnabled(true);
				}*/

				emit sendPcOnOff(true);
			}
			});
	}
	_pcon->show();

	
}

PcOnOffContainer::~PcOnOffContainer()
{}

void PcOnOffContainer::setUncheck(bool pcoff)
{
	if (_pcon->isChecked()) {
		_pcon->setEnabled(true);
		_pcon->setChecked(false);
	}

	//if (pcoff) {
	//	if (_pcoff->isChecked()) {
	//		_pcoff->setEnabled(true);
	//		_pcoff->setChecked(false);
	//	}

	//}
	//else {
	//	if (_pcon->isChecked()) {
	//		_pcon->setEnabled(true);
	//		_pcon->setChecked(false);
	//	}
	//}
}

void PcOnOffContainer::setUncheckAll()
{
	/*if (_pcoff->isChecked()) {
		_pcoff->setEnabled(true);
		_pcoff->setChecked(false);
	}*/

	if (_pcon->isChecked()) {
		_pcon->setEnabled(true);
		_pcon->setChecked(false);
	}
}
