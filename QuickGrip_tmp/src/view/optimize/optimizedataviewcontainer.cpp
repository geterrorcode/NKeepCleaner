#include "optimizedataviewcontainer.h"

#include <QTreeview>
#include <QTreeWidget>

OptimizeDataViewContainer::OptimizeDataViewContainer(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(400, 300);


    //auto _viewTreeWidget = new QTreeWidget;
    //_viewTreeWidget->setAllColumnsShowFocus(true);
    //_viewTreeWidget->setUniformRowHeights(true);
    //_viewTreeWidget->setItemsExpandable(false);
    //_viewTreeWidget->setRootIsDecorated(false);
    //_viewTreeWidget->setDragEnabled(false);
    //_viewTreeWidget->setSortingEnabled(false);
    //_viewTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //_viewTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //_viewTreeWidget->setFocusPolicy(Qt::NoFocus);

    //_viewTreeWidget->setStyleSheet(qss("ConfigDownFolderDialog:Widget:TreeWidget").arg(QObject::tr("Arial"), SGUP_PT_DPI_SCALE(9)));

    /*_viewTreeWidget->setHeader(new HeaderView(Qt::Horizontal, _viewTreeWidget));
    _viewTreeWidget->header()->setMinimumHeight(32);
    _viewTreeWidget->header()->setStyleSheet(qss("ConfigDownFolderDialog:Widget:TreeWidgetHeader").arg(QObject::tr("Arial"), SGUP_PT_DPI_SCALE(9)));
    _viewTreeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _viewTreeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);*/


}

OptimizeDataViewContainer::~OptimizeDataViewContainer()
{}
