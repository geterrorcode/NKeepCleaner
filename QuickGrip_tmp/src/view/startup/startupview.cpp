#include "startupview.h"

#include <QTableWidget>
#include <QCheckBox>
#include <QFileIconProvider>

#include "../../../QuickGrip_tmp.h"

#include "view/Component/vlayout.h"
#include "view/Component/button.h"
#include "view/Component/tablecheckbox.h"
#include "view/common/alertpopup.h"

#include "../Container/topinfowidget.h"


#include "../../controller/startup/startupcontroller.h"
//#include "../../model/startup/startupdata.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"
#include "../../common/tool/utils.h"

StartUpView::StartUpView(QWidget *parent)
	: QWidget(parent) , _table(Q_NULLPTR), _delbtn(Q_NULLPTR)
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


	//phonenum->setText("電話でのお問い合わせ :  <font color=#FDBB30>050-55526-9859</font>");
	//設定された
	auto toplabel = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
	toplabel->setGeometry(258 - marginsize_x, 98 - marginsize_y, 710, 24);
	toplabel->setAlignment(Qt::AlignCenter);
	toplabel->setText(QString("<font color=#FDBB30>設定された</font>スタートプログラムをリストから削除できます"));
	toplabel->show();


	//desc
	auto desclabel = new CustomLabel(18, "#FFFFFF", false, 0, 90, this);
	desclabel->setText("※スタートプログラムとはPC起動時に自動で起動されるプログラムのことで、\r\n    自分もしくはプログラム自体から設定されます。\r\n"
		"    スタートプログラムが多いとPCの起動が遅くなることがあります。\r\n※以下の項目から使用しないプログラムを削除することができます。");
	desclabel->setGeometry(100, 150 - marginsize_y, 700, 100);
	desclabel->setWordWrap(true);
	desclabel->show();

	_table = new QTableWidget(this);
	_table->setColumnCount(3);
	_table->setColumnWidth(0, 20);
	_table->setColumnWidth(1, 400 );
	//_table->setColumnWidth(2, 200);
	//_table->setColumnWidth(3, 100);

	QStringList headers;
	headers.append("削除");
	headers.append("プログラム名");
	headers.append("設置された経路");
	//headers.append("設定タイプ");

	_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
	_table->setHorizontalHeaderLabels(headers);
	_table->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#282A2E; color:#FFFFFF;}");
	_table->setStyleSheet(ssdefines::tablewidget);
	_table->setFrameStyle(QFrame::NoFrame);
	_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	_table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	_table->horizontalHeader()->setStretchLastSection(true);
	_table->verticalHeader()->hide();
	_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	_table->setSelectionMode(QAbstractItemView::SingleSelection);

	_table->setGeometry(258 - marginsize_x, 248 - marginsize_y, 710, 309);
	_table->show();

	QObject::connect(_table, &QTableWidget::cellClicked, [=](int row, int column) {

		if (row >= 0) {
			//TableCheckBox

			auto checker = (TableCheckBox*)_table->cellWidget(row, 0);
			if (checker) {
				if (checker->isChecked())
					checker->setChecked(false);
				else
					checker->setChecked(true);
			}
		}

		});

	auto refresh = new Button(this);
	QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/refreshbtn.png")).arg("QPushButton{color:#ffffff; font-weight:bold;}");
	refresh->setStyleSheet(ss);
	refresh->setText("リストを更新");
	refresh->setGeometry(450 - marginsize_x, 582 - marginsize_y, 153, 40);
	refresh->show();
	connect(refresh, &Button::clicked, [=]() {
		emit signalgetStartup();
		auto alert = new AlertPopup(tr("更新が完了しました"), QObject::tr("ok"), "", false, window());
		alert->show();

		});

	auto del = new Button(this);
	ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{color:#23262A; font-weight:bold;}");
	del->setStyleSheet(ss);
	del->setText("削除 ");
	del->setGeometry(623 - marginsize_x, 582 - marginsize_y, 153, 40);
	del->show();
	connect(del, &Button::clicked, [=]() {
		//_table->selec

		QList<int> selindex = {};
		for (size_t i = 0; i < _table->rowCount(); i++)
		{
			auto tablecheckbox = (TableCheckBox*)_table->cellWidget(i, 0);
			if (tablecheckbox->isChecked()) {
				selindex.append(i);
			}
		}
		
		if (selindex.size() == 0) {
			auto alert = new AlertPopup(tr("削除する項目を選択してください"), QObject::tr("ok"), "", false, window());
			alert->show();
			return;
		}

		{

			auto alert = new AlertPopup(tr("選択されたスタートプログラムは完全に削除されます。\r\n削除しますか？"), QObject::tr("削除"), QObject::tr("cancel"), true, window());
			connect(alert, &AlertPopup::okClicked, [=]() {
				emit signaldelStartup(selindex);
				});

			connect(alert, &AlertPopup::cancelClicked, [=]() {
				return;
				});

			alert->show();
		}
		

		//emit signaldelStartup();
		});

	setFixedSize(parent->size());

	QObject::connect(this, SIGNAL(signalgetStartup()), StartupController::inst(), SLOT(slotgetStartupData()));
	QObject::connect(StartupController::inst(), 
		SIGNAL(signalsetStartUpData(QVector<StartUpData>::iterator, QVector<StartUpData>::iterator)),
		this, 
		SLOT(slotupdateStartupList(QVector<StartUpData>::iterator , QVector<StartUpData>::iterator )));

	QObject::connect(this, SIGNAL(signaldelStartup(QList<int>)),
		StartupController::inst(), SLOT(slotdelStartup(QList<int>)));

	QObject::connect(StartupController::inst(), &StartupController::signalsetStartDelComplete, this, [=]() {
		auto alert = new AlertPopup(tr("選択したスタートプログラムの設定を削除しました"), QObject::tr("ok"), "", false, window());
		alert->show();

		});
}

StartUpView::~StartUpView()
{
	/*if (_startup_controller.get())
		delete _startup_controller*/;
}

void StartUpView::setListupdate()
{
	emit signalgetStartup();
}

void StartUpView::getStartUp()
{
	emit signalgetStartup();
}

void StartUpView::slotupdateStartupList(QVector<StartUpData>::iterator h, QVector<StartUpData>::iterator e)
{
	try
	{
		_table->clear();

		QStringList headers;
		headers.append("削除");
		headers.append("プログラム名");//プログラム名
		headers.append("設置された経路");
		//headers.append("設定タイプ");

		_table->setHorizontalHeaderLabels(headers);
		_table->setRowCount(0);

		int tableindex = 0;
		while (h != e)
		{
			
			//int index = h.key();
			//StartUpData data = h.value();
			//StartUpData data(&h);
			StartUpData data = *h;
			
			if (data.namne.isEmpty())
				continue;

			auto item = new QTableWidgetItem();
			item->setText(data.namne);
			if (data.type == StartUpType::regType) {

				QString buf = data.file_path.replace("\"", "");
				QString exepath = buf.left(buf.lastIndexOf(".exe") + 4);
				if (!exepath.isEmpty()) {
					QIcon icon = utils::getIconFromFile(exepath);
					if (!icon.isNull())
						item->setIcon(icon);
				}
			}
			else {
				QIcon icon = utils::getIconFromFile(data.file_path);
				if (!icon.isNull())
					item->setIcon(icon);
			}


			_table->insertRow(tableindex);

			auto tablecheckbox = new TableCheckBox();
			_table->setCellWidget(tableindex, 0, tablecheckbox);

			item->setForeground(QBrush("#A9ABAD"));
			_table->setItem(tableindex, 1, item);

			auto filepath = new QTableWidgetItem(data.file_path);
			filepath->setForeground(QBrush("#A9ABAD"));
			_table->setItem(tableindex, 2, filepath);

			//auto type = new QTableWidgetItem(typeToString(data.type));
			//type->setForeground(QBrush("#A9ABAD"));
			//type->setTextAlignment(Qt::AlignHCenter);
			//_table->setItem(tableindex, 3, type);

			

			tableindex++;
			h++;
		}
	}
	catch (const std::exception&)
	{

	}
}


QString StartUpView::typeToString(StartUpType type)
{
	if (type == StartUpType::fileType)
		return QString("スタートフォルダ");
	else
		return QString("レジストリ");
}
