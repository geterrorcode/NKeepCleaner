#include "downloadcontainer.h"

#include <QProgressBar>

//#include "../Component/customlabel.h"
#include "../ui.h"
#include "../downloader/client.h"


DownloadContainer::DownloadContainer(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	//setStyleSheet(ss("widget").arg(QString("Malgun Gothic")));

	auto baseLayout = new ContainerLayout(this);
	baseLayout->setContentsMargins(3, 3, 3, 3);
	{
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet("background-color:#2E3135; border:1px solid #cecece; border-radius:3px;");
		baseLayout->addWidget(backgroundFrame);
	}

	auto mainLayout = new VLayout(this);
	mainLayout->setContentsMargins(12, 30, 12, 10);
	baseLayout->addLayout(mainLayout);

	{
		// title>
		//auto title = new CommonLabel(17, "#6a615c", true);
		auto title = new CustomLabel(17, "#FFFFFF", true, 0, 0, this);
		title->setText(tr("N-KeepCleaner")); 
		title->setAlignment(Qt::AlignCenter);
		mainLayout->addWidget(title);

		mainLayout->addSpacing(3);

		auto body = new CustomLabel(17, "#FFFFFF", false, 0, 0, this);
		body->setText(tr("アップデート中です。")); // 업데이트 중입니다.
		body->setAlignment(Qt::AlignCenter);
		body->setFixedHeight(28);
		mainLayout->addWidget(body);


		mainLayout->addSpacing(16);

	}
	
	

	{
		// Text 
		auto percentLabel = new CustomLabel(17, "#FFFFFF",false,  0, 0, this);
		percentLabel->setAlignment(Qt::AlignCenter);
		percentLabel->setText("0%");

		mainLayout->addWidget(percentLabel);
		mainLayout->addSpacing(7);
		

		// prograss
		auto progress = new QProgressBar;
		progress->setContentsMargins(0, 0, 0, 0);
		progress->setFixedSize(260, 30);
		
		QString ss = "";

		progress->setStyleSheet("QProgressBar {background:#202224; border:none; border-radius:8px; color: white;}"
			"QProgressBar::chunk {background-color: #FDBB30; border-radius:8px;}");

		/*progress->setStyleSheet("QProgressBar {background:#202224; border:none; border-radius:8px; color: white;}"
			"QProgressBar::chunk {background-color: #FDBB30; border-radius:8px;}");*/

		//progress->setStyleSheet(ss("ProgressBar"));
		progress->setTextVisible(false);
		progress->setValue(0);
		mainLayout->addWidget(progress, 0 , Qt::AlignCenter);
		
		
	

		connect(Client::Inst().getDownLoader(), &DownLoader::siganlProgress, this, [=](qint64 recvSize, qint64 totalSize) {
			int percent = ((float)recvSize / (float)totalSize) * 100.f;

			qDebug() << "percent [" << percent << "] " << "recv : " << recvSize << "totalSize : " << totalSize;
			progress->setValue(percent);

			QString percentText = QString::number(percent);
			percentText += QString("%");

			percentLabel->setText(percentText);
		});

	}


	mainLayout->addStretch(1);




}
