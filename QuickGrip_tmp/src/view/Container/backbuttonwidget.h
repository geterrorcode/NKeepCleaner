#pragma once

#include <QObject>
#include <QWidget>

#include <QToolTip>
#include <QEvent>
#include <QMouseEvent>

class BackButtonWidget  : public QWidget
{
	Q_OBJECT

public:
	BackButtonWidget(QWidget *parent);
	~BackButtonWidget();

private:
	void mousePressEvent(QMouseEvent* event) override;
	bool event(QEvent* event) override;
Q_SIGNALS:
	void backButtonClicked();
};
