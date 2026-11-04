#include "optimizefilelist.h"

#include <QTreeview>
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QMap>

#include <view/Component/containerlayout.h>
#include <view/Component/customlabel.h>
#include <view/Component/button.h>
#include <view/common/makemodal.h>
#include <view/Component/dragger.h>
#include <view/Component/optimizefileshinteventhandler.h>

#include <model/optimize/fileoptimizer.h>
#include <model/optimize/ieoptimizer.h>
#include <model/optimize/regoptimizer.h>

#include <common/defines/ssdefins.h>
#include <common/tool/utils.h>
#include <common/defines/commondefine.h>


#include <ui.h>

OptimizeFileList::OptimizeFileList(OptimizeType type, bool optimizeafter, QWidget *parent)
	: QWidget(parent)
{
	makeModal(this);

	//setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(788, 472);

	auto baseLayout = new VLayout(this);
	{
		// <background
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet("background-color: #2E3135; border: 1px solid #cecece; border-radius: 3px;");
		baseLayout->addWidget(backgroundFrame);

	}

	auto topwidget = new QWidget(this);
	//topwidget->setContentsMargins(5, 5, 5, 0);
	topwidget->setFixedHeight(50);
	//topwidget->setStyleSheet("background-color: #2E3135; ");
	//topwidget->setStyleSheet(ssdefines::imageWidget.arg(":preview/resource/preview/top.png"));
	auto topLayout = new ContainerLayout(topwidget);
	auto dragger = new Dragger(this);
	dragger->setFocusPolicy(Qt::StrongFocus);
	topLayout->addWidget(dragger);
	topwidget->setGeometry(0, 0, 788, 50);

	auto closebtn = new Button(this);
	{
		closebtn->setFixedSize(30, 30);
		closebtn->setCursor(Qt::PointingHandCursor);
		closebtn->setStyleSheet(ssdefines::imageButton.arg(QString(":/main/resource/main/close.png"))
			.arg(":/main/resource/main/close_over.png")
			.arg(":/main/resource/main/close_press.png")
			.arg(":/main/resource/main/close.png"));
		
		connect(closebtn, &Button::clicked, [=]() {
			close();

			});
	}
	closebtn->setGeometry(748, 10, 30, 30);
	closebtn->show();


	auto data = new QWidget;
	data->setStyleSheet("background-color: #2E3135; border:1px solid #cecece; border-radius:3px;");
	//data->setStyleSheet("background-color: #2E3135; border: 0px; ");
	data->setFixedHeight(472 - 50);
	//baseLayout->addWidget(data);
	//baseLayout->addStretch(0);

	auto _viewTreeWidget = new QTreeWidget(this);
	_viewTreeWidget->setAllColumnsShowFocus(true);
	_viewTreeWidget->setUniformRowHeights(true);
	//_viewTreeWidget->setItemsExpandable(true);
	//_viewTreeWidget->setRootIsDecorated(false);
	_viewTreeWidget->setDragEnabled(false);
	_viewTreeWidget->setSortingEnabled(false); 
	_viewTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	_viewTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	_viewTreeWidget->setFocusPolicy(Qt::NoFocus);
	_viewTreeWidget->setHeader(new HeaderView(Qt::Horizontal, _viewTreeWidget));
	_viewTreeWidget->header()->setMinimumHeight(32);
	//_viewTreeWidget->setStyleSheet(ssdefines::customtree);
	QString ss = QString("QWidget { background-color: #2E3135; border:1px solid #444444; color:#FFFFFF;}"
		" QHeaderView::section { background-color: #2E3135; border-right:1px solid #444444;color:#FFFFFF;} "
		);
	_viewTreeWidget->header()->setStyleSheet(ss);

	auto titlelabel = new CustomLabel(18, "#FFFFFF", true, 0, 0, this);
	titlelabel->setGeometry(10, 15, 600, 30);
	QString titletext = "";
	QString heaername_path, heaername_size;

	if(optimizeafter) { //실패 목록
		if (type == OptimizeType::FileType) {

			heaername_path = "経路";
			heaername_size = "容量";
			titletext = QString("最適化失敗した <font color=#FDBB30>システムファイル</font> 項目です");

			QMap<FileOptimizeType, QStringList> info;
			QVector<fileOptimizeData>::iterator iter = FileOptimizer::inst()->getFileFailFirstIter();
			while (iter != FileOptimizer::inst()->getFileFailLastIter())
			{
				FileOptimizeType type = iter->type;
				QString path = iter->filepath;
				size_t size = iter->filesize;
				iter++;

				QStringList slItem;
				slItem << path << utils::getFileSizeType(size);
				info[type].append(slItem);
			}

			if (info.size() > 0)
			{
				for (size_t i = 0; i <= (int)FileOptimizeType::FileWinRecycleBin; i++)
				{
					FileOptimizeType type = (FileOptimizeType)i;
					int count = info[type].count();
					if (count > 0) {
						QTreeWidgetItem* topLevel = new QTreeWidgetItem();
						topLevel->setText(0, FileOptimizer::inst()->getFileTypeToString(type));
						for (size_t i = 0; i < count; i++)
						{
							QString path = info[type][i];
							i++;
							QString size = info[type][i];
							QTreeWidgetItem* item = new QTreeWidgetItem();
							item->setText(1, path);
							item->setText(2, size);
							topLevel->addChild(item);
						}
						QSize sz;
						sz.setHeight(19);
						sz.setWidth(490);
						topLevel->setSizeHint(0, sz);
						_viewTreeWidget->addTopLevelItem(topLevel);
					}
				}
			}
		}
		else if (type == OptimizeType::IEFileType) {

			heaername_path = "経路";
			heaername_size = "容量";

			titletext = QString("最適化失敗した <font color=#FDBB30>ブラウザキャッシュファイル</font> 項目です");

			QMap<FileOptimizeType, QStringList> info;
			QVector<fileOptimizeData>::iterator iter = IEOptimizer::inst()->getIEFailFirstIter();
			while (iter != IEOptimizer::inst()->getIEFailLastIter())
			{
				FileOptimizeType type = iter->type;
				QString path = iter->filepath;
				size_t size = iter->filesize;
				iter++;

				QStringList slItem;
				slItem << path << utils::getFileSizeType(size);
				info[type].append(slItem);
			}

			if (info.size() > 0)
			{
				for (size_t i = (int)FileOptimizeType::EdgeCache; i < (int)FileOptimizeType::none; i++)
				{
					FileOptimizeType type = (FileOptimizeType)i;
					int count = info[type].count();
					if (count > 0) {
						QTreeWidgetItem* topLevel = new QTreeWidgetItem();
						topLevel->setText(0, IEOptimizer::inst()->getIEFileTypeToString(type));
						for (size_t i = 0; i < count; i++)
						{
							QString path = info[type][i];
							i++;
							QString size = info[type][i];
							QTreeWidgetItem* item = new QTreeWidgetItem();
							item->setText(1, path);
							item->setText(2, size);
							topLevel->addChild(item);
						}

						QSize sz;
						sz.setHeight(19);
						sz.setWidth(490);
						topLevel->setSizeHint(0, sz);
						_viewTreeWidget->addTopLevelItem(topLevel);
					}
				}
			}
		}
		else if (type == OptimizeType::RegType) {

			heaername_path = "レジストリ名";
			heaername_size = "経路";

			titletext = QString("最適化失敗した<font color=#FDBB30>レジストリ</font> 項目です");

			QMap<RegOptimizeType, QStringList> info;
			QList<regOptimizeData>::iterator iter = RegOptimizer::inst()->getRegFailFirstIter();
			while (iter != RegOptimizer::inst()->getRegFailLastIter())
			{
				QStringList slItem;
				slItem << iter->regpath << iter->regvalue;
				//slItem << iter->keyname << iter->regpath;
				info[iter->optimizetype].append(slItem);

				iter++;
			}

			if (info.size() > 0)
			{
				for (size_t i = (int)RegOptimizeType::RegWrongAppPath; i < (int)RegOptimizeType::none; i++)
				{
					RegOptimizeType type = (RegOptimizeType)i;
					int count = info[type].count();

					if (count > 0) {
						QTreeWidgetItem* topLevel = new QTreeWidgetItem();
						topLevel->setText(0, RegOptimizer::inst()->getRegTypeToString(type));
						for (size_t i = 0; i < count; i++)
						{
							QString name = info[type][i];
							i++;
							QString path = info[type][i];
							QTreeWidgetItem* item = new QTreeWidgetItem();
							item->setText(1, name);
							item->setText(2, path);
							topLevel->addChild(item);
						}
						QSize sz;
						sz.setHeight(19);
						sz.setWidth(490);
						topLevel->setSizeHint(0, sz);
						_viewTreeWidget->addTopLevelItem(topLevel);
					}
				}
			}
		}
	}
	else{
		if (type == OptimizeType::FileType) {

			heaername_path = "経路";
			heaername_size = "容量";

			titletext = QString("最適化する <font color=#FDBB30>システムファイル</font>スキャン項目です");

			QMap<FileOptimizeType, QStringList> info;
			QVector<fileOptimizeData>::iterator iter = FileOptimizer::inst()->getFileScanFirstIter();
			while (iter != FileOptimizer::inst()->getFileScanLastIter())
			{
				FileOptimizeType type = iter->type;
				QString path = iter->filepath;
				size_t size = iter->filesize;
				iter++;

				QStringList slItem;
				slItem << path << utils::getFileSizeType(size);
				info[type].append(slItem);
			}

			if (info.size() > 0)
			{
				for (size_t i = 0; i < (int)FileOptimizeType::FileWinRecycleBin; i++)
				{
					FileOptimizeType type = (FileOptimizeType)i;
					int count = info[type].count();
					if (count > 0) {
						
						QTreeWidgetItem* topLevel = new QTreeWidgetItem();
						topLevel->setText(0, FileOptimizer::inst()->getFileTypeToString(type));
						

						//Qt::CheckState checkState = FileOptimizer::inst()->getFileTypeToCheck(type);
						if(FileOptimizer::inst()->isOptimizeType(type))
							topLevel->setCheckState(0, Qt::Checked);
						else
							topLevel->setCheckState(0, Qt::Unchecked);
						

						//OptimizeListInfo
						OptimizeListInfo listdata{(uint)OptimizeType::FileType , (uint)type, FileOptimizer::inst()->getFileTypeToOptimizeDesc(type) };
						QVariant listDataVariant;
						listDataVariant.setValue(listdata); //struct to Variant

						topLevel->setData(0, Qt::UserRole, listDataVariant);
						
						for (size_t i = 0; i < count; i++)
						{
							QString path = info[type][i];
							i++;
							QString size = info[type][i];
							QTreeWidgetItem* item = new QTreeWidgetItem();
							item->setText(1, path);
							item->setText(2, size);
							topLevel->addChild(item);
						}
						QSize sz;
						sz.setHeight(19);
						sz.setWidth(490);
						topLevel->setSizeHint(0, sz);
						_viewTreeWidget->addTopLevelItem(topLevel);
					}
				}
			}
			//휴지통 파일 정보 출력
			size_t recyleBinCnt = FileOptimizer::inst()->getRecyleBinCount();
			if (recyleBinCnt > 0) {

				QTreeWidgetItem* topLevel = new QTreeWidgetItem();
				topLevel->setText(0, FileOptimizer::inst()->getFileTypeToString(FileOptimizeType::FileWinRecycleBin));

				//Qt::CheckState checkState = FileOptimizer::inst()->getFileTypeToCheck(FileOptimizeType::FileWinRecycleBin);
				if (FileOptimizer::inst()->isOptimizeType(FileOptimizeType::FileWinRecycleBin))
					topLevel->setCheckState(0, Qt::Checked);
				else
					topLevel->setCheckState(0, Qt::Unchecked);
				/*if (checkState == Qt::Checked)
					FileOptimizer::inst()->updateOptimizeState(FileOptimizeType::FileWinRecycleBin, true);
				else
					FileOptimizer::inst()->updateOptimizeState(FileOptimizeType::FileWinRecycleBin, false);*/

				//OptimizeListInfo
				OptimizeListInfo listdata{ (uint)OptimizeType::FileType , 
					(uint)FileOptimizeType::FileWinRecycleBin, 
					FileOptimizer::inst()->getFileTypeToOptimizeDesc(FileOptimizeType::FileWinRecycleBin) };
				QVariant listDataVariant;
				listDataVariant.setValue(listdata); //struct to Variant

				topLevel->setData(0, Qt::UserRole, listDataVariant);
				QTreeWidgetItem* item = new QTreeWidgetItem();
				item->setText(1, QString("削除可能なファイル数 : %1").arg(FileOptimizer::inst()->getRecyleBinCount())); //삭제 가능한 파일 갯수 
				item->setText(2, QString("%1").arg(FileOptimizer::inst()->getRecyleBinSize()));
				topLevel->addChild(item);

				QSize sz;
				sz.setHeight(19);
				sz.setWidth(490);
				topLevel->setSizeHint(0, sz);
				_viewTreeWidget->addTopLevelItem(topLevel);
			}
			//_viewTreeWidget->setStyleSheet(ssdefines::customtree_test);
			//_viewTreeWidget->setStyleSheet(_viewTreeWidget->styleSheet());
		}
		else if (type == OptimizeType::IEFileType) {

			heaername_path = "経路";
			heaername_size = "容量";
			titletext = QString("最適化する <font color=#FDBB30>ブラウザキャッシュファイル</font>スキャン項目です");

			QMap<FileOptimizeType, QStringList> info;
			QVector<fileOptimizeData>::iterator iter = IEOptimizer::inst()->getIEScanFirstIter();
			while (iter != IEOptimizer::inst()->getIEScanLastIter())
			{
				
				FileOptimizeType type = iter->type;
				QString path = iter->filepath;
				size_t size = iter->filesize;
				iter++;

				QStringList slItem;
				slItem << path << utils::getFileSizeType(size);
				info[type].append(slItem);
			}

			if (info.size() > 0)
			{
				for (size_t i = (int)FileOptimizeType::EdgeCache; i < (int)FileOptimizeType::none; i++)
				{
					FileOptimizeType type = (FileOptimizeType)i;
					int count = info[type].count();
					if (count > 0) {
						QTreeWidgetItem* topLevel = new QTreeWidgetItem();
						topLevel->setText(0, IEOptimizer::inst()->getIEFileTypeToString(type));
						
						//Qt::CheckState checkState = IEOptimizer::inst()->getIEFileTypeToCheck(type);
						if (IEOptimizer::inst()->isIEOptimizeType(type))
							topLevel->setCheckState(0, Qt::Checked);
						else
							topLevel->setCheckState(0, Qt::Unchecked);

						//(uint)type, FileOptimizer::inst()->getFileTypeToDesc(type) };
						OptimizeListInfo listdata{ (uint)OptimizeType::IEFileType ,
							(uint)type, IEOptimizer::inst()->getIEFileTypeToOptimizeDesc(type) };
							
						QVariant listDataVariant;
						listDataVariant.setValue(listdata); //struct to Variant

						topLevel->setData(0, Qt::UserRole, listDataVariant);
						//topLevel->setText(1, IEOptimizer::inst()->getIEFileTypeToOptimizeDesc(type));

						for (size_t i = 0; i < count; i++)
						{
							QString path = info[type][i];
							i++;
							QString size = info[type][i];
							QTreeWidgetItem* item = new QTreeWidgetItem();
							item->setText(1, path);
							item->setText(2, size);
							topLevel->addChild(item);
						}
						QSize sz;
						sz.setHeight(19);
						sz.setWidth(490);
						topLevel->setSizeHint(0, sz);
						_viewTreeWidget->addTopLevelItem(topLevel);
					}
				}
			}
		}
		else if (type == OptimizeType::RegType) {

			heaername_path = "レジストリ名";
			heaername_size = "経路";
			titletext = QString("最適化する <font color=#FDBB30>レジストリ</font>スキャン項目です");

			QMap<RegOptimizeType, QStringList> info;
			QMap<int, regOptimizeData>::iterator iter = RegOptimizer::inst()->getRegScanFirstIter();
			while (iter != RegOptimizer::inst()->getRegScanLastIter())
			{
				regOptimizeData data = iter.value();
				iter++;


				QStringList slItem;
				slItem << data.regpath << data.regvalue;
				info[data.optimizetype].append(slItem);
			}

			if (info.size() > 0)
			{
				for (size_t i = (int)RegOptimizeType::RegWrongAppPath; i < (int)RegOptimizeType::none; i++)
				{
					RegOptimizeType type = (RegOptimizeType)i;
					int count = info[type].count();

					if (count > 0) {
						QTreeWidgetItem* topLevel = new QTreeWidgetItem();
						topLevel->setText(0, RegOptimizer::inst()->getRegTypeToString(type));
						for (size_t i = 0; i < count; i++)
						{
							QString name = info[type][i];
							i++;
							QString path = info[type][i];
							QTreeWidgetItem* item = new QTreeWidgetItem();
							item->setText(1, name);
							item->setText(2, path);
							topLevel->addChild(item);
						}
						QSize sz;
						sz.setHeight(19);
						sz.setWidth(490);
						topLevel->setSizeHint(0, sz);
						_viewTreeWidget->addTopLevelItem(topLevel);
					}
				}
			}

		}

	}

	

	titlelabel->setText(titletext);
	titlelabel->show();

	QStringList slHeader({ "タイプ", heaername_path, heaername_size });
	_viewTreeWidget->setColumnCount(slHeader.count());
	_viewTreeWidget->setHeaderLabels(slHeader);
	_viewTreeWidget->header()->resizeSection(0, 300);
	_viewTreeWidget->header()->resizeSection(1, 360);
	//_viewTreeWidget->header()->resizeSection(2, 80);

	_viewTreeWidget->setGeometry(10, 55, 768, 410);

	// 이벤트 필터 객체 생성 및 설치(툴팁)
	auto* filter = new OptimizeFilesHintEventHandler(this);
	_viewTreeWidget->viewport()->installEventFilter(filter);
	_viewTreeWidget->setStyleSheet(ssdefines::customtree);
	_viewTreeWidget->show();

	// 항목이 변경될 때 호출될 슬롯 연결
	connect(_viewTreeWidget, &QTreeWidget::itemChanged, this, &OptimizeFileList::onItemChanged);

}

void OptimizeFileList::onItemChanged(QTreeWidgetItem* item, int column)
{
	bool use = false;
	if (item->checkState(column) == Qt::Checked)
		use = true;

	if (item) {
		QVariant tooltipData = item->data(0, Qt::UserRole);
		if (tooltipData.isValid()) {

			auto extractedStruct = item->data(0, Qt::UserRole).value<OptimizeListInfo>();
			OptimizeType optiType = (OptimizeType)extractedStruct.optimizeType;
			FileOptimizeType detileType = (FileOptimizeType)extractedStruct.optimizeDetailType;
			
			if(optiType == OptimizeType::FileType){
				FileOptimizer::inst()->updateOptimizeState(detileType, use);
			}
			else if (optiType == OptimizeType::IEFileType) {
				IEOptimizer::inst()->updateIEOptimizeState(detileType, use);
			}

		}
	}


	//getStringToFileType
	//FileOptimizeType type = FileOptimizer::inst()->getStringToFileType(item->text(0));

	
}

OptimizeFileList::~OptimizeFileList()
{}
