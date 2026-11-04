#pragma once

#include <QObject>
#include <QWidget>
#include <QPointer>
#include <QCheckBox>
#include <QVector>
#include <QMap>
#include <QList>

#include "view/Component/customlabel.h"

#include "../../model/smart_optimize/smartopimizedata.h"

class SmartOptimizeView  : public QWidget
{
	Q_OBJECT

public:
	SmartOptimizeView(QWidget *parent);
	~SmartOptimizeView();

private:
	QPointer<CustomLabel> _listempty_label;
	QMap<int, QCheckBox*> _reserve_lists;

	QVector<QRect> list_pos;

	
	void updateReserveList();


Q_SIGNALS:
	void getSmartOptimizeData();
	void setSmartOptimizePcOnOffData(bool, bool);
	void setSmartOptimizeData(int, ReserveData);
	void deleteSmartOptimizeData(QList<int> list);
};
