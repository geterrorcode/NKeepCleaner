#include "appwizview.h"

#include <QTableWidget>
#include <QCheckBox>
#include <QDebug>

#include "view/Component/vlayout.h"
#include "view/Component/button.h"
#include "view/Component/customlabel.h"
#include "view/common/alertpopup.h"

#include "view/Component/QtListView.h"
#include "view/Component/QtListViewImpl.h"

#include "../Container/topinfowidget.h"

#include "../../controller/appwiz/appwizcontroller.h"


#include "../../ui.h"
#include "../../common/defines/ssdefins.h"
#include "../../common/tool/utils.h"


AppwizView::AppwizView(QWidget *parent)
	: QWidget(parent), _table(Q_NULLPTR)
{

	int marginsize_x = 218;
	int marginsize_y = 50;

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
	toplabel->setText(QString("インストールされたプログラムを選択し<font color=#FDBB30>アンインストール</font>できます"));
	toplabel->show();

	//710
	_table = new QTableWidget(this);
	_table->setColumnCount(6);
	_table->setColumnWidth(0, 200);
	_table->setColumnWidth(1, 100);
	_table->setColumnWidth(2, 120);
	_table->setColumnWidth(3, 80);
	_table->setColumnWidth(4, 100);

	QStringList headers;
	headers.append("プログラム名");
	headers.append("会社");
	headers.append("インストール日");
	headers.append("容量");
	headers.append("バージョン");
	headers.append("アンインストール");

	_table->setHorizontalHeaderLabels(headers);
	_table->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#282A2E;color:#FFFFFF;}");
	_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeMode::Stretch);
	//_table->setStyleSheet("background-color: #282AE; border: none; ");
	_table->setStyleSheet(ssdefines::tablewidget);
	
	//QTableWidget::item:selected{ background - color: red }
	_table->setFrameStyle(QFrame::NoFrame);
	_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	_table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	//_table->horizontalHeader()->setStretchLastSection(true);
	_table->verticalHeader()->hide();
	_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	//_table->setGeometry(258 - marginsize_x, 188 - marginsize_y, 710, 369); //원본
	_table->setGeometry(250 - marginsize_x, 188 - marginsize_y, 726, 369); 
	_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	_table->setSelectionMode(QAbstractItemView::SingleSelection);

	_table->show();

	auto refresh = new Button(this);

	QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/refreshbtn.png")).arg("QPushButton{color:#ffffff; font-weight:bold;}");

	refresh->setStyleSheet(ss);
	refresh->setGeometry(510 - marginsize_x, 582 - marginsize_y, 153, 40);
	refresh->setText("リストを更新");
	refresh->show();
	connect(refresh, &Button::clicked, [=]() {
		setListupdate();

		auto alert = new AlertPopup(tr("更新が完了しました"), QObject::tr("ok"), "", false, window());
		alert->show();
		});

	setFixedSize(parent->size());

	QObject::connect(this, SIGNAL(signalgetAppwizData()), AppwizController::inst(), SLOT(slotgetAppwizData()));
	QObject::connect(AppwizController::inst(),
		SIGNAL(signalsetAppwizData(QMap<int, AppWizData*>::iterator, QMap<int, AppWizData*>::iterator)),
		this,
		SLOT(slotupdateAppwizList(QMap<int, AppWizData*>::iterator, QMap<int, AppWizData*>::iterator)));

	QObject::connect(this, SIGNAL(signaldelAppwizData(int)),
		AppwizController::inst(), SLOT(slotdelAppwizData(int)));

	QObject::connect(AppwizController::inst(), SIGNAL(signaldelAppwizdataResult(int)), this, SLOT(slotdellResult(int)));

	QObject::connect(AppwizController::inst(), &AppwizController::signalAppwizAlertCall, [=]() {
		auto alert = new AlertPopup(tr("既に削除されたプログラムです。更新ボタンを押してください。"), QObject::tr("ok"), "", false, window());
		alert->show();
		});

	//QObject::connect(this, SIGNAL(signalViewChange(viewIndex)), parent, SLOT(selectBtnType(viewIndex)));
}

AppwizView::~AppwizView()
{}



void AppwizView::slotupdateAppwizList(QMap<int, AppWizData*>::iterator h, QMap<int, AppWizData*>::iterator e)
{
	try
	{
		_table->clear();
		int tableindex = 0;

		QStringList headers;
		headers.append("プログラム名");
		headers.append("会社");
		headers.append("インストール日");
		headers.append("容量");
		headers.append("バージョン");
		headers.append("アンインストール");
		_table->setHorizontalHeaderLabels(headers);
		_table->setRowCount(0);

		//_table->setStyleSheet("background-color: #282A2E; border: none; ");
		while (h != e)
		{
			int index = h.key();
			auto data = h.value();
			h++;

			if (!data)
				continue;

			_table->insertRow(tableindex);

			auto item = new QTableWidgetItem();
			item->setForeground(QBrush("#A9ABAD"));
			
			//QTableWidget::item:selected{ background - color: red }

			QIcon icon = utils::getIconFromFile(data->iconpath);
			if (!icon.isNull()) {
				item->setIcon(icon);
			}

			item->setText(data->program_name);
			_table->setItem(tableindex, 0, item);

			auto publisher = new QTableWidgetItem(data->publisher);
			publisher->setForeground(QBrush("#A9ABAD"));
			_table->setItem(tableindex, 1, publisher);

			auto installdate = new QTableWidgetItem(data->install_date);
			installdate->setForeground(QBrush("#A9ABAD"));
			installdate->setTextAlignment(Qt::AlignRight);
			_table->setItem(tableindex, 2, installdate);

			
			auto size = new QTableWidgetItem(data->size);
			size->setForeground(QBrush("#A9ABAD"));
			size->setTextAlignment(Qt::AlignRight);
			_table->setItem(tableindex, 3, size);

			//5.14.2 (14578)
			//QString versiondata = data->version.replace("\n", ""); //개행 제거
			auto version = new QTableWidgetItem(data->version);
			version->setForeground(QBrush("#A9ABAD"));
			version->setTextAlignment(Qt::AlignRight);
			_table->setItem(tableindex, 4, version);

			auto delbtn = new Button();
			{ //del
				delbtn->setText("削除"); //삭제
				delbtn->setStyleSheet("QAbstractButton {color:#A9ABAD; font: normal normal 14px;}");
				delbtn->setCursor(Qt::PointingHandCursor);
				delbtn->setFixedHeight(24);
				delbtn->setStyleSheet(ssdefines::imageButton.arg(":preview/resource/preview/okbtn.png",
					":preview/resource/preview/okbtn.png",
					":preview/resource/preview/okbtn.png",
					":preview/resource/preview/okbtn.png"));
				delbtn->setObjectName(QString::number(index));
				connect(delbtn, &Button::clicked, [=]() {

					auto alert = new AlertPopup(tr("削除選択したプログラムから提供されている\r\nアンインストーラーを起動します"
						"\r\n該当のアンインストーラーを通してプログラムを\r\n削除することができます。"
						"\r\n\r\nアンインストーラーを起動しますか？"), QObject::tr("起動"), QObject::tr("cancel"), true, window());
					connect(alert, &AlertPopup::okClicked, [=]() {
						emit signaldelAppwizData(delbtn->objectName().toInt());
						});
					alert->show();
					 
					});
			}
			_table->setCellWidget(tableindex, 5, delbtn);

			tableindex++;
			
		}
	}
	catch (const std::exception&)
	{

	}

	
}

void AppwizView::slotdellResult(int key)
{
	for (size_t i = 0; i < _table->rowCount(); i++)
	{
		auto delbtn = (Button*)_table->cellWidget(i, 5);
		if (delbtn) {
			if (delbtn->objectName().toInt() == key) {
				_table->removeRow(i);
				_table->update();
				return;
			}
		}
	}
}

void AppwizView::setListupdate()
{
	emit signalgetAppwizData();
}

