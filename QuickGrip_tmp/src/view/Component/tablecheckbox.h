#pragma once

#include <QWidget>
#include <QCheckBox>

class TableCheckBox  : public QWidget
{
	Q_OBJECT

public:
	TableCheckBox(QWidget *parent = Q_NULLPTR);
	~TableCheckBox();

public:
	bool isChecked();
	void setChecked(bool check);

private :
	bool _check_status;
	QCheckBox* _checkbox;
};
