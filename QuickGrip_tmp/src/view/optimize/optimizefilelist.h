#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QHeaderView>

#include <model/optimize/optimizdata.h>

class HeaderView : public QHeaderView
{
public:
    explicit HeaderView(Qt::Orientation orientation, QWidget* parent = 0)
        : QHeaderView(orientation, parent)
    {
        this->setStyleSheet("background-color:#2E3135;");
        setStretchLastSection(true);
        setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    QSize sizeHint() const
    {
        return QHeaderView::sizeHint().expandedTo(minimumSize());
    }
};

class OptimizeFileList  : public QWidget
{
	Q_OBJECT

public:
	OptimizeFileList(OptimizeType type, bool optimizeafter = false, QWidget *parent = Q_NULLPTR);
	~OptimizeFileList();


    void onItemChanged(QTreeWidgetItem* item, int column);
};
