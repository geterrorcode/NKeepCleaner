#pragma once

#include <QObject>
#include <QMap>
#include <QList>

#include "smartopimizedata.h"

#include "../../common/ux/singleton.h"

class SmartOptimizeInfo  : public QObject, public common::ux::Singleton<SmartOptimizeInfo>
{
	Q_OBJECT

public:
	SmartOptimizeInfo(QObject *parent = Q_NULLPTR);
	~SmartOptimizeInfo();

private:
	QMap<int, ReserveData> _smartinfo;
	QString _cfgpath;
	bool updateData();
	void initJsonData();
	void parseJsonData(const QJsonObject& json);
	
	
public:
	QMap<int, ReserveData>::iterator getFirstIter() {
		return _smartinfo.begin();
	}

	QMap<int, ReserveData>::iterator getLastIter() {
		return _smartinfo.end();
	}

	bool getUseData();
	bool isBootingOptimize() const;
	bool isPcOffOptimize() const;

signals:
	void reserveUpdateComplete();
	void reserveDelComplete();

public slots:
	void setReserve(int key, ReserveData data, bool del = false);
	void delReserve(QList<int> list);
	
};
