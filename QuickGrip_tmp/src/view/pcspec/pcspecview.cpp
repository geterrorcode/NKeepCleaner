#include "pcspecview.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

#include "../../controller/pcinfo/pcinfocontroller.h"

#include "../Component/customlabel.h"
#include "../Component/copylabel.h"
#include "../Component/button.h"
#include "../common/alertpopup.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"
#include "../../common/tool/utils.h"

PcSpecView::PcSpecView(QWidget *parent)
	: QWidget(parent)
{

	int marginsize_x = 218;
	int marginsize_y = 50;

	auto baseLayout = new ContainerLayout(this);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
		baseLayout->addWidget(backgroundFrame);
	}

	auto body = new QLabel(this);
	body->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/gbbody.png"));
	body->setGeometry(259 - marginsize_x, 76 - marginsize_y, 713, 564);
	body->show();

	//cpu
	auto cpu_text = new CustomLabel(18, "#FFFFFF", true, 0,0, this);
	cpu_text->setText("プロセッサー");
	cpu_text->setGeometry(310 - marginsize_x, 131 - marginsize_y, 120, 16);
	cpu_text->show();

	auto cpu_icon = new QLabel(this);
	cpu_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_cpu.png"));
	cpu_icon->setGeometry(317 - marginsize_x, 158 - marginsize_y, 15, 15);
	cpu_icon->show();

	auto cpudata= PcInfoController::inst()->getCPUInfo();
	auto cpu = new CopyLabel(14, false, "#A9ABAD", this);
	cpu->setGeometry(342 - marginsize_x, 155 - marginsize_y, 700, 30);
	cpu->setText(cpudata);
	cpu->show();

	//gpu
	auto gpu_text = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	gpu_text->setText("グラフィックカード");
	gpu_text->setGeometry(310 - marginsize_x, 198 - marginsize_y, 180, 16);
	gpu_text->show();

	auto gpu_icon = new QLabel(this);
	gpu_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_gpu.png"));
	gpu_icon->setGeometry(317 - marginsize_x, 225 - marginsize_y, 15, 15);
	gpu_icon->show();

	auto gpudata = PcInfoController::inst()->getGPUInfo();
	auto gpu = new CopyLabel(14, false, "#A9ABAD", this);
	gpu->setGeometry(342 - marginsize_x, 224 - marginsize_y, 600, 30);
	gpu->setText(gpudata);
	gpu->show();

	//ram
	auto ram_text = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	ram_text->setText("メモリ");
	ram_text->setGeometry(310 - marginsize_x, 265 - marginsize_y, 150, 16);
	ram_text->show();

	auto ram_icon = new QLabel(this);
	ram_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_mem.png"));
	ram_icon->setGeometry(317 - marginsize_x, 292 - marginsize_y, 15, 15);
	ram_icon->show();

	auto ramdata = PcInfoController::inst()->getRamInfo();
	auto ram = new CopyLabel(14, false, "#A9ABAD", this);
	ram->setGeometry(342 - marginsize_x, 291 - marginsize_y, 500, 30);
	ram->setText(ramdata);
	ram->show();

	//display
	auto dis_text = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	dis_text->setText("解像度");
	//dis_text->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/text_display.png"));
	dis_text->setGeometry(310 - marginsize_x, 332 - marginsize_y, 150, 16);
	dis_text->show();

	auto dis_icon = new QLabel(this);
	dis_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_dis.png"));
	dis_icon->setGeometry(317 - marginsize_x, 359 - marginsize_y, 15, 15);
	dis_icon->show();

	auto displaydata = PcInfoController::inst()->getDisplayInfo();
	auto display = new CopyLabel(14, false, "#A9ABAD", this);
	display->setGeometry(342 - marginsize_x, 358 - marginsize_y, 500, 30);
	display->setText(displaydata);
	display->show();

	//mac
	auto mac_text = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	mac_text->setText("物理アドレス(MACアドレス)");
	mac_text->setGeometry(310 - marginsize_x, 399 - marginsize_y, 350, 16);
	mac_text->show();

	auto mac_icon = new QLabel(this);
	mac_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_mac.png"));
	mac_icon->setGeometry(317 - marginsize_x, 426 - marginsize_y, 15, 15);
	mac_icon->show();

	auto macdata = PcInfoController::inst()->getMacAddress();
	auto mac = new CopyLabel(14, false, "#A9ABAD", this);
	mac->setGeometry(342 - marginsize_x, 425 - marginsize_y, 500, 30);
	mac->setText(macdata);
	mac->show();

	//ipaddress
	
	auto ip_text = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	ip_text->setText("IPアドレス(IP)");
	ip_text->setGeometry(310 - marginsize_x, 466 - marginsize_y, 131, 16);
	ip_text->show();

	auto ip_icon = new QLabel(this);
	ip_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_ip.png"));
	ip_icon->setGeometry(317 - marginsize_x, 492 - marginsize_y, 15, 15);
	ip_icon->show();

	auto ipdata = PcInfoController::inst()->getIPAddress();
	auto ip = new CopyLabel(14, false, "#A9ABAD", this);
	ip->setGeometry(342 - marginsize_x, 491 - marginsize_y, 500, 30);
	ip->setText(ipdata);
	ip->show();

	//os
	auto os_text = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	os_text->setText("プラットフォーム");
	os_text->setGeometry(310 - marginsize_x, 533 - marginsize_y, 160, 16);
	os_text->show();

	auto os_icon = new QLabel(this);
	os_icon->setStyleSheet(ssdefines::imageWidget.arg(":/pcspec/resource/preview/pcspec/icon_os.png"));
	os_icon->setGeometry(317 - marginsize_x, 560 - marginsize_y, 15, 15);
	os_icon->show();

	auto osdata = PcInfoController::inst()->getOSInfo();
	auto os = new CopyLabel(14, false, "#A9ABAD", this);
	os->setGeometry(342 - marginsize_x, 559 - marginsize_y, 500, 30);
	os->setText(osdata);
	os->show();

	auto textsave = new Button(this);
	textsave->setText("情報ファイルとして保存");
	QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{color:#23262A; font-weight:bold;}");
	textsave->setStyleSheet(ss);
	textsave->setGeometry(marginsize_x + 320, 40  , 210, 40);
	textsave->show();
	connect(textsave, &Button::clicked, [=]() {
		QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

		QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File", documentsPath, "Text Files (*.txt)");
		if (!fileName.isEmpty()) {
			// Create a file
			QFile file(fileName);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				// Write text to the file
				QTextStream out(&file);
				out << "[マイPC仕様]\r\n\r\n";
				out << QString("※ プロセッサー : %1\r\n").arg(cpudata);
				out << QString("※ グラフィックカード : %1\r\n").arg(gpudata);
				out << QString("※ メモリ : %1\r\n").arg(ramdata);
				out << QString("※ 解像度 : %1\r\n").arg(displaydata);
				out << QString("※ 物理アドレス(MACアドレス) : %1\r\n").arg(macdata);
				out << QString("※ IPアドレス(IP) : %1\r\n").arg(ipdata);
				out << QString("※ プラットフォーム : %1\r\n").arg(osdata);
				file.close();

				auto alert = new AlertPopup("保存されました", QObject::tr("ok"), "", false, window());
				alert->show();
			}
		}

		});
	
	setFixedSize(parent->size());
}

PcSpecView::~PcSpecView()
{

}
