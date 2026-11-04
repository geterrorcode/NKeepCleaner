#pragma once

#include <QObject>
#include <QtWidgets/QMainWindow>

class MainView  : public QMainWindow
{
	Q_OBJECT

public:
	MainView(QWidget *parent);
	~MainView();
};
