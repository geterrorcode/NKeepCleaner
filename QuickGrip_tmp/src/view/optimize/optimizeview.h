#pragma once

#include <QObject>
#include <QWidget>
#include <QProgressBar>

#include "optimizeresulwidget.h"
#include "optimizecompleteresultwidget.h"

#include "view/Component/button.h"
#include "view/Component/customlabel.h"

#include "../../model/optimize/optimizdata.h"
#include "../../common/defines/defineproperty.h"

class OptimizeView  : public QWidget
{
	Q_OBJECT

	PROPERTY(int, marginsizeX, setMarginX)
	PROPERTY(int, marginsizeY, setMarginY)

public:
	OptimizeView(int marginx = 218, int marginy = 50, QWidget *parent = Q_NULLPTR);
	~OptimizeView();


private:
	bool _check_status;

	Button* _optimize_btn;
	QProgressBar* _progressBar;
	CustomLabel* _message;
	CustomLabel* _result_message;

	CustomLabel* _autoOptimize_label;

	OptimizeResultWidget* _result_system;
	OptimizeResultWidget* _result_ie;
	OptimizeResultWidget* _result_reg;


	OptimizeCompleteResultWidget* _completresult_system;
	OptimizeCompleteResultWidget* _completresult_ie; 
	OptimizeCompleteResultWidget* _completresult_reg;

	QLabel* _optimizestatus_gif;
	QLabel* _balloonImg;
	QLabel* _balloonImg2;

	void initResource();

	OptimizeLevel getLevel(int filecount, int filesize);

signals:
	void getOptimizeinfo();
	void optimize();

public slots:
	void updateProgress(int per);
	void updateMessage(QString Message);

	void sendClickedOptimize();

};
