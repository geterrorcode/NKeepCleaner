#pragma once

#include <QWidget>
#include <QMouseEvent>

#include "view/Component/customlabel.h"
//#include "ui_optimizeresulwidget.h"

class OptimizeResultWidget : public QWidget
{
	Q_OBJECT

public:
	OptimizeResultWidget(const QString imagpath, const QString title, QWidget *parent = nullptr);
	~OptimizeResultWidget();

	void setOptiText(QString count, QString size = Q_NULLPTR);
private:
	void mousePressEvent(QMouseEvent* event) override;

	//Ui::OptimizeResulWidgetClass ui;
	CustomLabel* _count;
	CustomLabel* _size;

Q_SIGNALS:
	void sendClicked();

};
