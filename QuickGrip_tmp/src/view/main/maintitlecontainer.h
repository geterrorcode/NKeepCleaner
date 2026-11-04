#pragma once

#include <QObject>
#include <QWidget>

class MainTitleContainer  : public QWidget
{
	Q_OBJECT

public:
	MainTitleContainer(QWidget *parent = Q_NULLPTR);
	~MainTitleContainer();
};
