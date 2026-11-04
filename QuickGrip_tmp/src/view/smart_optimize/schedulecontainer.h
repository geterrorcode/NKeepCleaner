#pragma once

#include <QWidget>
#include <QListView>

#include "view/Component/vlayout.h"

class ScheduleContainer  : public VLayout
{
	Q_OBJECT

public:
	ScheduleContainer(QWidget *parent = nullptr);
	~ScheduleContainer();

private:
	QWidget* _empty_image;
	QListView* _listview;

	void updateListData();

private slots:
	void listUpdate(bool showImage);
};
