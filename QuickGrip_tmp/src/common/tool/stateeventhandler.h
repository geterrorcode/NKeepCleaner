#pragma once

#include <QObject>
#include <QMainWindow>
#include <QEvent>
#include <QDebug>

class StateEventHandler  : public QMainWindow
{
	Q_OBJECT

public:
	StateEventHandler(QWidget *parent = Q_NULLPTR);
	~StateEventHandler();

protected:
	void changeEvent(QEvent* event) override;
};
