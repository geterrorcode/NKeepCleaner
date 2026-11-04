#include "optionview.h"

#include <QCheckBox>

#include "view/Component/vlayout.h"
#include "view/Component/button.h"
#include "view/Component/customlabel.h"
#include "view/common/alertpopup.h"

#include "view/Container/topinfowidget.h"

#include "../../model/configdata/configdata.h"

#include "../../../appversion.h"
#include "../../ui.h"
#include "../../common/defines/ssdefins.h"



OptionView::OptionView(QWidget *parent)
	: QWidget(parent), _startup_opt(false), _tray_opt(false)
{

	int marginsize_x = 218;
	int marginsize_y = 50;

	_tray_opt = ConfigData::inst()->getTrayOpt();
	_startup_opt = ConfigData::inst()->getStartupOpt();

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
	toplabel->setGeometry(258 - marginsize_x, 98 - marginsize_y, 710, 24);
	toplabel->setAlignment(Qt::AlignCenter);
	toplabel->setText(QString("Pogram Version : %1  <font color=#FDBB30>Update : %2</font>").arg(FILE_VERSION_STR).arg(UPDATEDATE));
	toplabel->show();

	auto body = new QLabel(this);
	body->setStyleSheet(ssdefines::imageWidget.arg(":/option/resource/preview/option/gbbody.png"));
	body->setGeometry(258 - marginsize_x, 175 - marginsize_y, 713, 465);
	body->show();

	//종료시 트레이로
	auto strayopt_check = new QCheckBox(this);
	strayopt_check->setStyleSheet(ssdefines::checkboxbigImage);
	//strayopt_check->setGeometry(313 - marginsize_x, 346 - marginsize_y, 250, 20);
	strayopt_check->setGeometry(313 - marginsize_x, 230 - marginsize_y, 500, 20);
	strayopt_check->setCheckable(true);
	strayopt_check->setChecked(_tray_opt);
	
	strayopt_check->setText(QString("終了時トレイに移動"));
	strayopt_check->show();
	connect(strayopt_check, &QCheckBox::clicked, [=]() {

		if (strayopt_check->isChecked()) {
			//emit settrayopt(true);
			ConfigData::inst()->setTrayOpt(true);
		}
		else {
			//emit settrayopt(false);
			ConfigData::inst()->setTrayOpt(false);
		}

		});
	auto trayoptdesc = new CustomLabel(15, "#FDFDFD", false, 0, 60, this);
	trayoptdesc->setWordWrap(this);
	trayoptdesc->setText("※N-KeepCleaner終了後、PC画面右下の隠れているインジケータ―に移動します");
	trayoptdesc->setGeometry(328 - marginsize_x, 250 - marginsize_y, 650, 80);
	//trayoptdesc->setGeometry(328 - marginsize_x, 376 - marginsize_y, 650, 80);
	trayoptdesc->show();

	//=====================================================


	/*auto funcoptionbg = new QLabel(this);
	funcoptionbg->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/summary.png"));
	funcoptionbg->setGeometry(280 - marginsize_x, 350 - marginsize_y, 690, 285);
	funcoptionbg->show();*/


	//레지스트리 최적화 옵션
	//auto regopt_check = new QCheckBox(this);
	//regopt_check->setStyleSheet(ssdefines::checkboxbigImage_smallfont);
	//regopt_check->setGeometry(313 - marginsize_x, 360 - marginsize_y, 350, 20);
	//regopt_check->setCheckable(true);
	//regopt_check->setChecked(_tray_opt);
	//regopt_check->setText(QString("最適化時のレジストリ項目有効化"));//최적화 시 레지스트리 항목 추가
	//regopt_check->show();
	//connect(regopt_check, &QCheckBox::clicked, [=]() {

	//	if (regopt_check->isChecked()) {
	//		//emit settrayopt(true);
	//		ConfigData::inst()->setRegOptimizeOpt(true);
	//	}
	//	else {
	//		//emit settrayopt(false);
	//		ConfigData::inst()->setRegOptimizeOpt(false);
	//	}

	//	});

	//auto regoptdesc = new CustomLabel(16, "#FDFDFD", false, 0, 60, this);
	//regoptdesc->setWordWrap(this);
	//regoptdesc->setText("※レジストリ項目はPCに多くの影響を与える項目です。\r\n    一般的には無効にすることを推奨します。");
	//regoptdesc->setGeometry(328 - marginsize_x, 380 - marginsize_y, 650, 20);
	//regoptdesc->show();


	////시작프로그램 정리 옵션
	//auto startupopt_check = new QCheckBox(this);
	//startupopt_check->setStyleSheet(ssdefines::checkboxbigImage_smallfont);
	//startupopt_check->setGeometry(313 - marginsize_x, 445 - marginsize_y, 350, 20);
	//startupopt_check->setCheckable(true);
	//startupopt_check->setChecked(_tray_opt);
	//startupopt_check->setText("PC起動時に自動で実行される項目を整理する機能有効化(スタートプログラム有効化)");//PC기동시 자동 실행 목록 정리 기능 활성화
	//startupopt_check->show();
	//connect(startupopt_check, &QCheckBox::clicked, [=]() {
	//	if (startupopt_check->isChecked()) {
	//		//emit settrayopt(true);
	//		ConfigData::inst()->setStartProgramOpt(true);
	//	}
	//	else {
	//		//emit settrayopt(false);
	//		ConfigData::inst()->setStartProgramOpt(false);
	//	}
	//	});

	//auto startupoptdesc = new CustomLabel(16, "#FDFDFD", false, 0, 60, this);
	//startupoptdesc->setWordWrap(this);
	//startupoptdesc->setText("※PC起動時に自動で実行される項目を間違って削除していまうと\r\n    PC起動に問題が生じる可能性があります。\r\n    削除が必要であり問題ないと判断する項目のみ選択及び削除してください。");
	//startupoptdesc->setGeometry(328 - marginsize_x, 475 - marginsize_y, 650, 80);
	//startupoptdesc->show();


	////프로그램 제거 옵션
	//auto uninstallopt_check = new QCheckBox(this);
	//uninstallopt_check->setStyleSheet(ssdefines::checkboxbigImage_smallfont);
	//uninstallopt_check->setGeometry(313 - marginsize_x, 545 - marginsize_y, 350, 20);
	//uninstallopt_check->setCheckable(true);
	//uninstallopt_check->setChecked(_tray_opt);
	//uninstallopt_check->setText("プログラム削除機能有効化"); //프로그램 제거 기능 추가
	//uninstallopt_check->show();
	//connect(uninstallopt_check, &QCheckBox::clicked, [=]() {
	//	if (uninstallopt_check->isChecked()) {
	//		//emit settrayopt(true);
	//		ConfigData::inst()->setUninstallOpt(true);
	//	}
	//	else {
	//		//emit settrayopt(false);
	//		ConfigData::inst()->setUninstallOpt(false);
	//	}
	//	});

	//auto uninstalloptdesc = new CustomLabel(16, "#FDFDFD", false, 0, 60, this);
	//uninstalloptdesc->setWordWrap(this);
	//uninstalloptdesc->setText("※PCにインストールされているすべてのプログラム項目が表示されます。\r\n    必ず必要なプログラムは削除しないように正確に確認し、選択及び削除をしてください。");
	//uninstalloptdesc->setGeometry(328 - marginsize_x, 565 - marginsize_y, 650, 80);
	//uninstalloptdesc->show();

	setFixedSize(parent->size());


}

OptionView::~OptionView()
{}
