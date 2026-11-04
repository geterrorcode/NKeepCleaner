#pragma once

#include <QObject>
#include <Windows.h>

#include "../../../QuickGrip_tmp.h"
#include "../../model/optimize/optimizdata.h"
#include "../../common/ux/singleton.h"

class OptimizeController  : public QObject,
	public common::ux::Singleton<OptimizeController>
{
	Q_OBJECT

public:
	OptimizeController(QObject *parent = Q_NULLPTR);
	~OptimizeController();

private:
	bool _filecheck;
	bool _iecheck;
	bool _regcheck;

	bool _scanning;
	bool _optimizing;
	bool _automode;

	LONGLONG _allcount;
	LONGLONG _checkcount;

	FileOptimizeType _filetype;
	RegOptimizeType _regtype;



	void sendprogress(bool end = false);

	void openTrayPopup(QString msg);

public:
	void getSystemCheckValue(QString& count, QString& size);
	void getSystemFailValue(size_t& count, size_t& size);
	
	void getIECheckValue(QString& count, QString& size);
	void getIEFailValue(size_t& count, size_t& size);
	void getRegFailValue(size_t& count);

	QString getRegCheckValue();
	QString getRegOptimizeValue();

	void setScanDate();

signals:
	void optimizeCount();
	void fileSearchCount();
	void ieSarchCount();
	void regSearchCount();
	void sendStatusMsg(QString msg);

	void startFilecheck();
	void startIEcheck();
	void startRegcheck();

	void sendper(int);

	void allcheckfinished();
	void allOptimizefinished();
	void setScanTime();


	void startFileOptimize();
	void startIEFileOptimize();
	void startRegOptimize();

	void clickOtpBtn();

	void setInitPage();

	void callTraypopupTest();
	void callTraypopup(QString);

public slots:
	void setOptimize();
	void optimizeCountEnd();
	void getMessage(QString msg);

	void startOptimize();

	void hideOptimize(bool lightver);

	void pcoffOptimize();
};
