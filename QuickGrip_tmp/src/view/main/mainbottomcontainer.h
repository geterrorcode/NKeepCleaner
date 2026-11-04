#pragma once

#include <QWidget>
#include <QPushButton>

#include <QToolTip>
#include <QEvent>
#include <QMouseEvent>

#include "../../view/Component/customlabel.h"

class MainBottomContainer  : public QPushButton
{
	Q_OBJECT

public:
	MainBottomContainer(QString imgpath, QString name, QString data, QString tooltip, QWidget *parent = Q_NULLPTR);
	~MainBottomContainer();

	void setUpdate(QString imgpath, QString data);

private:
	QWidget* _img;
	CustomLabel* _bottom_data;
	QString _imgpath;
	QString _data;
	QString _tooltip;

private:
	bool event(QEvent* event) override;
};
