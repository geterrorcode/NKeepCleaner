#pragma once

#include <QObject>
#include <QWidget>
#include <QKeyEvent>

class AuthExpiredKeyPopup : public QWidget
{
	Q_OBJECT

public:
	AuthExpiredKeyPopup(QString serialcode, QWidget* parent = Q_NULLPTR);
	~AuthExpiredKeyPopup();

protected:
	void keyPressEvent(QKeyEvent* event) override;

signals:
	void okClicked();
	void callBilling();
	void reStart();
	void cancelClicked();
	

};
