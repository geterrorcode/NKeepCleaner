#pragma once

#include <QWidget>

class OptionView  : public QWidget
{
	Q_OBJECT

public:
	OptionView(QWidget *parent = Q_NULLPTR);
	~OptionView();


private:
	bool _startup_opt;
	bool _tray_opt;

signals:
	void settrayopt(bool tray);
};
