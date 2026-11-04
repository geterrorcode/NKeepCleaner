#pragma once

#include <QWidget>
//#include "ui_serialbackupmailview.h"

class SerialBackupMailView : public QWidget
{
	Q_OBJECT

public:
	SerialBackupMailView(QString serialcode, QWidget *parent = nullptr);
	~SerialBackupMailView();

private:

protected:
	//void keyPressEvent(QKeyEvent* event) override;
	//Ui::SerialBackupMailViewClass ui;
};
