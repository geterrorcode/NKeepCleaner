#pragma once

#include <QObject>
#include <QWidget>
#include <QKeyEvent>


//#include "ui_AuthSerialKeyResetPopup.h"

class AuthSerialKeyResetPopup : public QWidget
{
	Q_OBJECT

public:
	AuthSerialKeyResetPopup(QString serialcode, QWidget *parent = Q_NULLPTR);
	~AuthSerialKeyResetPopup();

protected:
	void keyPressEvent(QKeyEvent* event) override;

signals:
	void okClicked();
	void cancelClicked();

};
