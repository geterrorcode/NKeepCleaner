#pragma once

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

#include "../../common/defines/defineproperty.h"

class ClickableLabel  : public QLabel
{
	Q_OBJECT

public:
	explicit ClickableLabel(QWidget* parent = Q_NULLPTR);
	//ClickableLabel(QObject *parent);
	~ClickableLabel();

signals:
	void clicked();

protected:
	//void mousePressEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event) override;
};
