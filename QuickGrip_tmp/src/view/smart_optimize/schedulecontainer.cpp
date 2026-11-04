#include "schedulecontainer.h"

#include <QComboBox>
#include <QAbstractItemView>
#include <QListView>
#include <vector>

#include "view/Component/hlayout.h"
#include "view/Component/button.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

ScheduleContainer::ScheduleContainer(QWidget *parent)
	: VLayout(parent), _empty_image(Q_NULLPTR), _listview(Q_NULLPTR)
{
	//setContentsMargins(6, 36, 0, 6);
	setSpacing(6);

	auto bg = new QWidget();
	bg->setStyleSheet("border: 2px solid #666666; border-radius: 15px;");
	bg->setFixedHeight(300);
	this->addWidget(bg);

	auto mainlayout = new VLayout(bg);
	{
		mainlayout->setContentsMargins(10, 10, 10, 0);

		auto label = new QLabel;
		label->setText("設定済みのリスト");
		//label->setStyleSheet("border: none; background: transparent;");
		label->setStyleSheet("border: 1px solid #ffffff; background: transparent;");
		label->setFixedHeight(30);
	
		mainlayout->addWidget(label);
		//mainlayout->setAlignment(label, Qt::AlignTop);

		auto listlayout = new ContainerLayout();
		{
			auto backgroundFrame = new QFrame;
			backgroundFrame->setStyleSheet("background-color: #000fff; border: 1px solid #363c48; ");
			listlayout->addWidget(backgroundFrame);

			auto detail = new VLayout(backgroundFrame);

			if (!_empty_image) {
				_empty_image = new QWidget();
				_empty_image->setStyleSheet(ssdefines::imageWidget.arg(":/smartoptimize/resource/smartoptimize/empty_image.png"));
				//_empty_image->hide();
				//_empty_image->setStyleSheet("border: 1px solid #ffffff; background: transparent;");
				//_empty_image->setFixedSize(40, 40);
				_empty_image->setFixedSize(80, 80);

				//mainlayout->addWidget(_empty_image);
				detail->addWidget(_empty_image);
				detail->setAlignment(_empty_image, Qt::AlignHCenter);
				_empty_image->hide();
			}

			{
				//list
				_listview = new QListView(backgroundFrame);
				_listview->setContentsMargins(10, 20, 0, 0);
				//_listview->setMaximumSize(50, 100);
				_listview->setFixedWidth(180);
				_listview->setSpacing(5);
				_listview->setSelectionMode(QAbstractItemView::MultiSelection);
				_listview->setStyleSheet("background-color: #ffffff;");
				updateListData();

				auto deletebtn = new Button();
				deletebtn->setText("削除"); //삭제
				connect(deletebtn, &Button::clicked, [=]() {
					/*Q_FOREACH(const QModelIndex & index, _listview) {

					}*/
					});

				detail->addWidget(_listview);
				detail->addWidget(deletebtn);

			}

		}
		
		
		mainlayout->addLayout(listlayout);
		
		//mainlayout->addStretch(0);
	}

}

ScheduleContainer::~ScheduleContainer()
{}

void ScheduleContainer::updateListData()
{
	QStandardItemModel* poModel = new QStandardItemModel(_listview);

	std::vector<QString> data;
	//data.push_back("pcoff");
	//data.push_back("pcon");
	//data.push_back("1시 30분");
	//data.push_back("2시 30분");

	int i = 0;
	for (auto value : data)
	{
		QStandardItem* poListItem = new QStandardItem;
		poListItem->setCheckable(true);
		poListItem->setData(Qt::Unchecked, Qt::CheckStateRole);
		poListItem->setText(value);
		poModel->setItem(i, poListItem);

		i++;
	}

	_listview->setModel(poModel);
}

void ScheduleContainer::listUpdate(bool showImage)
{
	if (showImage) {

	}
	else {

	}
}